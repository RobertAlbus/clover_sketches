// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "implot.h"

#include "context.hpp"

#include "controllers.hpp"

#include "gui/component/gui_logger_component.hpp"
#include "gui/component/mixer_ui.hpp"
#include "gui/component/peq.hpp"
#include "lib/fdn/draw_fdn.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include <print>

void controller_mixer(const char* id, context& ctx);
void controller_kick(const char* id, context& ctx);
void controller_chord(const char* id, context& ctx);
void controller_chord_echoverb(const char* id, context& ctx);

std::vector<tabbed_controller> tabbed_controllers{
        // clang-format off
        {"mixer", controller_mixer},
        {"kick",  controller_kick},
        {"chord", controller_chord},
        {"chord echoverb", controller_chord_echoverb}
        // clang-format on
};

void controller_mixer(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);

    gui_logger_component(ctx);

    static double x = 0.5f;
    static double y = 0.5f;

    static bool show_plot = false;
    ImGui::Checkbox("show plot", &show_plot);
    if (show_plot) {
        if (ImPlot::BeginPlot("Draggable Point")) {
            ImPlot::DragPoint(0, &x, &y, ImVec4(1, 0, 0, 1), 4, ImPlotDragToolFlags_None);
            ImPlot::EndPlot();
        }
    }

    mixer_component("new_mix", &graph.mixer_tracks);
    peq_gui("##master_peq", graph.main_eq);

    ImGui::PopID();
}

void controller_kick(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);

    draw_kick_drum_000("kick_synth", graph.kick);
    if (ImGui::BeginTable("##kick_table", 2)) {
        ImGui::TableNextColumn();
        peq_gui("##kick_preverb_peq", graph.kick_preverb_peq);
        ImGui::TableNextColumn();
        ImGui::Text("kick postverb peq");
        peq_gui("##kick_out_peq", graph.kick_out_peq);
        ImGui::EndTable();
    }
    fdn_component("##kick_fdn", &graph.kick_verb, nullptr);

    ImGui::PopID();
}

void controller_chord(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);
    fdn_component("fdn", &graph.chord_verb_L, &graph.chord_verb_R);
    if (ImGui::BeginTable("##peq_table", 2)) {
        ImGui::TableNextColumn();
        ImGui::Text("chord preverb peq");
        peq_gui("##preverb_peq", graph.chord_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("chord out peq");
        peq_gui("##postverb_peq", graph.chord_peq);

        ImGui::EndTable();
    }
    ImGui::PopID();
}

void controller_chord_echoverb(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);
    peq_gui("##eq", graph.chord_echoverb.eq);
    ImGui::NewLine();
    if (ImGui::Button("set same as fdn_R")) {
        graph.chord_echoverb.verb_L.patch(graph.chord_echoverb.verb_R.props);
    }
    fdn_component("fdn_L", &graph.chord_echoverb.verb_L, nullptr);

    ImGui::NewLine();
    if (ImGui::Button("set same as fdn_L")) {
        graph.chord_echoverb.verb_R.patch(graph.chord_echoverb.verb_L.props);
    }
    fdn_component("fdn_R", &graph.chord_echoverb.verb_R, nullptr);

    ImGui::NewLine();
    if (ImGui::Button("copy echoverb_022_props")) {
        ImGui::SetClipboardText(graph.chord_echoverb.props.to_str().c_str());
    }
    float delay_samples_min = 4;
    float delay_samples_max = graph.chord_echoverb.max_length_samples - 4;

    ImGui::PushItemWidth(65);
    ImGui::DragFloat(
            "##delay samples  drag",
            &graph.chord_echoverb.props.delay_samples,
            1,
            delay_samples_min,
            delay_samples_max);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::SliderFloat(
            "delay samples", &graph.chord_echoverb.props.delay_samples, delay_samples_min, delay_samples_max);

    auto delay_bars           = (float)graph.grid.samples_to_bars(graph.chord_echoverb.props.delay_samples);
    const auto delay_bars_min = (float)graph.grid.samples_to_bars(delay_samples_min);
    const auto delay_bars_max = (float)graph.grid.samples_to_bars(delay_samples_max);

    ImGui::PushItemWidth(65);
    if (ImGui::DragFloat("##delay bars drag", &delay_bars, 1, delay_bars_min, delay_bars_max)) {
        auto as_samples = (float)graph.grid.bars_to_samples(delay_bars);
        as_samples      = std::clamp(as_samples, delay_samples_min, delay_samples_max);

        graph.chord_echoverb.props.delay_samples = as_samples;
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::SliderFloat("delay bars", &delay_bars, delay_bars_min, delay_bars_max)) {
        auto as_samples = (float)graph.grid.bars_to_samples(delay_bars);
        as_samples      = std::clamp(as_samples, delay_samples_min, delay_samples_max);

        graph.chord_echoverb.props.delay_samples = as_samples;
    }

    ImGui::SliderFloat("fb", &graph.chord_echoverb.props.fb, 0, 1.2);
    ImGui::SliderFloat("verb wet", &graph.chord_echoverb.props.verb_wet, 0, 2);
    ImGui::SliderFloat("verb dry", &graph.chord_echoverb.props.verb_dry, 0, 2);
    ImGui::PopID();
}
