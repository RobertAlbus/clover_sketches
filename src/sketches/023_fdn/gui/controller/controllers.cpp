// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "imgui.h"
#include "implot.h"

#include "instruments/fdn/draw_fdn.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/logging/draw_gui_log_canvas.hpp"
#include "lib/mixer/draw_mixer.hpp"

#include "context.hpp"
#include "controllers.hpp"

#include "lib/peq/draw_peq.hpp"

void controller_mixer(const char* id, context& ctx);
void controller_kick(const char* id, context& ctx);
void controller_chord(const char* id, context& ctx);

std::vector<tabbed_controller> tabbed_controllers{
        // clang-format off
        {"mixer", controller_mixer},
        {"kick",  controller_kick},
        {"chord",  controller_chord},
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
    draw_fdn8_023("##kick_fdn", &graph.kick_verb, nullptr);

    ImGui::PopID();
}

void controller_chord(const char* id, context& ctx) {
    signal_graph& graph = ctx.graph;

    ImGui::PushID(id);
    draw_fdn8_023("fdn", &graph.chord_verb_L, &graph.chord_verb_R);
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
