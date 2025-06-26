// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "imgui.h"
#include "lib/fdn/fdn8_023.hpp"
using namespace ImGui;

#include "draw_fdn8_023.hpp"

bool draw_tap_control(float& tap, float tap_min, float tap_max, float tap_range, float available_width) {
    PushID(&tap);

    // safety variable so that the underlying control cannot be coerced to invalid values
    float local_tap_value = tap;

    auto drag_fmt = tap < 1000.f ? "%.2f" : "%.f";
    PushItemWidth(available_width * 0.1f);
    bool changed_drag = DragFloat(
            "##tap_drag",
            &local_tap_value,
            local_tap_value * 0.01f,
            tap_min,
            tap_max,
            drag_fmt,
            ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    tap = std::clamp(local_tap_value, tap_min, tap_max);
    PopItemWidth();
    SameLine();
    PushItemWidth(available_width * 0.9f);
    float tap_slider_control = (tap - tap_min) / tap_range;

    bool changed_slider = SliderFloat(
            "##tap_slider",
            &tap_slider_control,
            0,
            1,
            "",
            ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    PopItemWidth();
    PopID();
    if (changed_slider || changed_drag) {
        local_tap_value = (tap_slider_control * tap_range) + tap_min;
        tap             = std::clamp(local_tap_value, tap_min, tap_max);
    }
    return changed_slider || changed_drag;
};

void draw_fdn8_023(const char* id, fdn8_023* fdn_L, fdn8_023* fdn_R) {
    PushID(id);
    if (ImGui::BeginTable("##fdn_table", 2)) {
        ImGui::TableNextColumn();
        if (ImGui::Button("get fdn patch")) {
            ImGui::SetClipboardText(fdn_L->props.to_str().c_str());
        }
        ImGui::NewLine();

        // one control for all fb gains
        if (SliderFloat("fb", &fdn_L->props.fb_gain, 0, 1.5) && fdn_R) {
            fdn_R->props.fb_gain = fdn_L->props.fb_gain;
        }

        // one control for all lpf filters
        if (SliderFloat(
                    "lpf cut",
                    &fdn_L->props.lpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            fdn_L->set_lpf(fdn_L->props.lpf_cut);
            if (fdn_R) {
                fdn_R->set_lpf(fdn_L->props.lpf_cut);
            }
        }

        // one control for all hpf filters
        if (SliderFloat(
                    "hpf cut",
                    &fdn_L->props.hpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            fdn_L->set_hpf(fdn_L->props.hpf_cut);
            if (fdn_R) {
                fdn_R->set_hpf(fdn_L->props.hpf_cut);
            }
        }

        ImGui::NewLine();

        ImGui::TableNextColumn();
        float drag_speed = ImGui::GetIO().KeyShift ? 0.01f : 1.0f;

        float tap_max   = fdn_L->fdls[0].m_max_idx;
        float tap_min   = 4;
        float tap_range = tap_max - tap_min;

        float available_width = ImGui::GetContentRegionAvail().x;

        std::array<float, 8>& taps_L = fdn_L->props.taps;
        std::array<float, 8>& taps_R = fdn_R ? fdn_R->props.taps : taps_L;
        for (auto [i, tap_L, tap_R] : std::views::zip(std::views::iota(0, 8), taps_L, taps_R)) {
            float tap_slider_control = (tap_L - tap_min) / tap_range;
            if (draw_tap_control(
                        tap_L,  //
                        tap_min,
                        tap_max,
                        tap_range,
                        available_width) &&
                fdn_R) {
                tap_R = tap_L;
            }
        }

        ImGui::EndTable();
    }
    PopID();
}
