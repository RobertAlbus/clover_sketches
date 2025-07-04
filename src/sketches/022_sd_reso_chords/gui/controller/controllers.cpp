// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "imgui.h"
#include "implot.h"

#include "lib/fdn/draw_fdn8_000.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/logging/draw_gui_log_canvas.hpp"
#include "lib/mixer_legacy/draw_mixer_000.hpp"

#include "context.hpp"
#include "controllers.hpp"

#include "lib/peq/draw_peq.hpp"

void controller_mixer(const char* id, context& ctx);
void controller_kick(const char* id, context& ctx);
void controller_chord(const char* id, context& ctx);
void controller_chord_echo(const char* id, context& ctx);

std::vector<tabbed_controller> tabbed_controllers{
        // clang-format off
        {"mixer", controller_mixer},
        {"kick",  controller_kick},
        {"chord", controller_chord},
        {"chord echo", controller_chord_echo}
        // clang-format on
};

void controller_mixer(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);
    static log_canvas_000 canvas;
    draw_gui_log_canvas_000("log_canvas", canvas, ctx.logger, nullptr);

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

    draw_mixer_000("new_mix", &graph.mixer_tracks);
    draw_peq_000("##master_peq", graph.main_eq);

    ImGui::PopID();
}

void controller_kick(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);

    draw_kick_drum_000("kick_synth", graph.kick);
    if (ImGui::BeginTable("##kick_table", 2)) {
        ImGui::TableNextColumn();
        draw_peq_000("##kick_preverb_peq", graph.kick_preverb_peq);
        ImGui::TableNextColumn();
        ImGui::Text("kick postverb peq");
        draw_peq_000("##kick_out_peq", graph.kick_out_peq);
        ImGui::EndTable();
    }
    draw_fdn8_000("##kick_fdn", &graph.kick_verb, nullptr);

    ImGui::PopID();
}

void controller_chord(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);
    draw_fdn8_000("fdn", &graph.chord_verb_L, &graph.chord_verb_R);
    if (ImGui::BeginTable("##peq_table", 2)) {
        ImGui::TableNextColumn();
        ImGui::Text("chord preverb peq");
        draw_peq_000("##preverb_peq", graph.chord_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("chord out peq");
        draw_peq_000("##postverb_peq", graph.chord_peq);

        ImGui::EndTable();
    }
    ImGui::PopID();
}

void controller_chord_echo(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);
    draw_peq_000("##eq", graph.chord_echo.eq);
    ImGui::NewLine();
    if (ImGui::Button("set same as fdn_R")) {
        graph.chord_echo_fb_verb_L.patch(graph.chord_echo_fb_verb_R.props);
    }
    draw_fdn8_000("fdn_L", &graph.chord_echo_fb_verb_L, nullptr);

    ImGui::NewLine();
    if (ImGui::Button("set same as fdn_L")) {
        graph.chord_echo_fb_verb_R.patch(graph.chord_echo_fb_verb_L.props);
    }
    draw_fdn8_000("fdn_R", &graph.chord_echo_fb_verb_R, nullptr);

    ImGui::NewLine();
    if (ImGui::Button("copy echoverb_022_props")) {
        ImGui::SetClipboardText(graph.chord_echo.props.to_str().c_str());
    }
    float delay_samples_min = 4;
    float delay_samples_max = graph.chord_echo.max_length_samples - 4;

    ImGui::PushItemWidth(65);
    ImGui::DragFloat(
            "##delay samples  drag",
            &graph.chord_echo.props.delay_samples,
            1,
            delay_samples_min,
            delay_samples_max);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::SliderFloat(
            "delay samples", &graph.chord_echo.props.delay_samples, delay_samples_min, delay_samples_max);

    auto delay_bars           = (float)graph.grid.samples_to_bars(graph.chord_echo.props.delay_samples);
    const auto delay_bars_min = (float)graph.grid.samples_to_bars(delay_samples_min);
    const auto delay_bars_max = (float)graph.grid.samples_to_bars(delay_samples_max);

    ImGui::PushItemWidth(65);
    if (ImGui::DragFloat("##delay bars drag", &delay_bars, 1, delay_bars_min, delay_bars_max)) {
        auto as_samples = (float)graph.grid.bars_to_samples(delay_bars);
        as_samples      = std::clamp(as_samples, delay_samples_min, delay_samples_max);

        graph.chord_echo.props.delay_samples = as_samples;
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::SliderFloat("delay bars", &delay_bars, delay_bars_min, delay_bars_max)) {
        auto as_samples = (float)graph.grid.bars_to_samples(delay_bars);
        as_samples      = std::clamp(as_samples, delay_samples_min, delay_samples_max);

        graph.chord_echo.props.delay_samples = as_samples;
    }

    ImGui::SliderFloat("fb", &graph.chord_echo.props.fb, 0, 1.2);

    ImGui::SliderFloat("chord echo fbverb dry", &graph.audio_mixer.at("chord echo fbverb dry").get(), 0, 2);
    ImGui::SliderFloat("chord echo fbverb wet", &graph.audio_mixer.at("chord echo fbverb wet").get(), 0, 2);

    ImGui::PopID();
}
