#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <print>
#include <ranges>

#include "imgui-knobs.h"
#include "imgui.h"

#include "clover/dsp/filter.hpp"

#include "implot.h"
#include "lib/peq/peq.hpp"

#include "util/filter_response.hpp"

/*
# first iteration
- assume unidirectional control.
- can enhance later with observer vs. controller versions
- can create a dsp eq unit with mod targets and ui controls!
*/

static inline double transform_forward_log2(double v, void*) {
    return std::log2(v);
}

static inline double transform_inverse_log2(double v, void*) {
    return exp2(v);
}

struct gpeq_ui {
    const char* name;
    gpeq_ui(const char* name, size_t resolution, peq_000& peq)
        :  //
          name(name),
          peq(peq),
          computed(resolution),
          resolution(resolution) {
        gui_peq_props = peq.props;

        freqs.resize(resolution);
        log_spaced_freqs(freqs, 20, 24000);
        axis_ticks.reserve(133);
        for (auto i : std::views::iota(1, 12) | std::views::reverse) {
            float base_tick = 24000.f / std::exp2(float(i));

            float next_tick = 24000.f / std::exp2(float(i - 1));
            float delta     = next_tick - base_tick;
            for (auto j : std::views::iota(0, 12)) {
                float tick = base_tick + (delta * (float(j) / 10.f));
                if (tick >= 20)
                    axis_ticks.emplace_back(tick);
            }
        }
        axis_ticks.emplace_back(24000);

        for (auto& complex_response : complex_responses)
            complex_response.resize(resolution);
        cumulative_complex_response.resize(resolution);
        computed.angles.resize(resolution);
        computed.magnitudes.resize(resolution);

        magnitudes_cache.resize(resolution);

        update_all();
    }

    std::array<complex_response, peq_000::SIZE> complex_responses;
    complex_response cumulative_complex_response;
    std::vector<float> freqs;
    std::vector<double> axis_ticks;
    computed_response computed;

    std::array<peq_props_000, peq_000::SIZE> gui_peq_props;
    peq_000& peq;
    size_t resolution;

    std::vector<float> magnitudes_cache;

    void update() {
        if (gui_peq_props == peq.props)
            return;

        auto view = std::views::zip(
                gui_peq_props,
                peq.props,
                peq.filters,
                complex_responses,
                std::views::iota(0, int(peq_000::SIZE)));

        for (auto [gui_peq_prop, filter_prop, filter, complex_response, i] : view) {
            if (gui_peq_prop == filter_prop)
                continue;

            peq.set(i, gui_peq_prop);
            compute_complex_response(filter.m_coeffs, complex_response, freqs);
        }
        std::fill(cumulative_complex_response.begin(), cumulative_complex_response.end(), 1);
        for (auto [complex_response, gui_peq_prop] : std::views::zip(complex_responses, gui_peq_props)) {
            char msg[16] = "";
            if (gui_peq_prop.enabled) {
                for (auto [cz, z] : std::views::zip(cumulative_complex_response, complex_response))
                    cz *= z;
            }
        }

        compute_spectrum(cumulative_complex_response, computed);
        unwrap_phase(computed.angles);
    }

    void update_all() {
        auto view = std::views::zip(
                gui_peq_props,
                peq.props,
                peq.filters,
                complex_responses,
                std::views::iota(0, int(peq_000::SIZE)));
        for (auto [gui_peq_prop, filter_prop, filter, complex_response, i] : view) {
            peq.set(i, gui_peq_prop.freq, gui_peq_prop.reso, gui_peq_prop.gain);
            compute_complex_response(filter.m_coeffs, complex_response, freqs);
        }

        std::fill(cumulative_complex_response.begin(), cumulative_complex_response.end(), 1);
        for (auto [complex_response, gui_peq_prop] : std::views::zip(complex_responses, gui_peq_props)) {
            if (gui_peq_prop.enabled) {
                for (auto [cz, z] : std::views::zip(cumulative_complex_response, complex_response))
                    cz *= z;
            }
        }
        compute_spectrum(cumulative_complex_response, computed);
        unwrap_phase(computed.angles);
    }

    bool draw() {
        const ImVec2 origin         = ImGui::GetCursorScreenPos();
        const bool was_changed      = draw_controls();
        const ImVec2 graph_position = ImVec2(origin.x + 250, origin.y);
        ImGui::SetCursorScreenPos(graph_position);
        draw_response();
        const ImVec2 end_position = ImVec2(origin.x, origin.y + 450);
        ImGui::SetCursorScreenPos(end_position);

        return was_changed;
    }

    void draw_response() {
        if (ImPlot::BeginPlot(name, ImVec2(-1, 0), ImPlotCond_Always)) {
            ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoTickLabels);
            ImPlot::SetupAxisTicks(ImAxis_X1, axis_ticks.data(), int(axis_ticks.size()));
            ImPlot::SetupAxisScale(ImAxis_X1, transform_forward_log2, transform_inverse_log2);
            ImPlot::PlotLine("magnitude", freqs.data(), computed.magnitudes.data(), (int)freqs.size());
            ImPlot::PlotLine(
                    "phase", freqs.data(), computed.angles.data(), (int)freqs.size(), ImPlotCond_Always);
            ImPlot::EndPlot();
        }
    }

    bool draw_controls() {
        bool was_changed = false;
        auto view        = std::views::zip(gui_peq_props, std::views::iota(0, int(peq_000::SIZE)));

        const ImVec2 control_origin = ImGui::GetCursorScreenPos();
        const float width           = 45;
        ImGui::PushItemWidth(width);
        for (auto [gui_peq_prop, i] : view) {
            ImGui::PushID(&gui_peq_prop);

            const float x_offset = control_origin.x + (55 * float(i));
            ImGui::SetCursorScreenPos(control_origin);

            ImGui::SetCursorPosX(x_offset);
            const char* current_type_str = peq_filter_str[int(gui_peq_prop.type)];
            if (ImGui::BeginCombo("##type", current_type_str)) {
                for (auto [type, str] : std::views::zip(peq_filter_types, peq_filter_str)) {
                    bool is_selected = (type == gui_peq_prop.type);
                    if (ImGui::Selectable(str, is_selected, 0)) {
                        gui_peq_prop.type = type;
                        was_changed       = true;
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SetCursorPosX(x_offset);
            was_changed |= ImGui::Checkbox("##enable", &gui_peq_prop.enabled);

            ImGui::SetCursorPosX(x_offset);
            was_changed |= ImGuiKnobs::Knob(
                    "freq",
                    &gui_peq_prop.freq,
                    10,
                    24000,
                    0,
                    "%.f",
                    ImGuiKnobVariant_Tick,
                    width,
                    ImGuiKnobFlags_Logarithmic);

            ImGui::SetCursorPosX(x_offset);
            bool is_ap           = gui_peq_prop.type == peq_filter_type::ap;
            const float reso_min = is_ap ? 0.0001 : 0.05;
            const float reso_max = is_ap ? 5 : 10;
            was_changed |= ImGuiKnobs::Knob(
                    "reso", &gui_peq_prop.reso, reso_min, reso_max, 0, "%.3f", ImGuiKnobVariant_Tick, width);

            ImGui::SetCursorPosX(x_offset);
            bool needs_gain = gui_peq_prop.type == peq_filter_type::eq ||
                              gui_peq_prop.type == peq_filter_type::hs ||
                              gui_peq_prop.type == peq_filter_type::ls;
            if (needs_gain) {
                was_changed |= ImGuiKnobs::Knob(
                        "gain", &gui_peq_prop.gain, -24, 24, 0, "%.3f", ImGuiKnobVariant_Tick, width);
            }

            if (was_changed)
                update();

            ImGui::PopID();
        }

        ImGui::PopItemWidth();
        return was_changed;
    }
};
