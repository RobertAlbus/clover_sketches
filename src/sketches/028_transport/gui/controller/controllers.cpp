// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/logging/draw_gui_log_canvas.hpp"
#include "lib/mixer/draw_mixer.hpp"
#include "lib/peq/draw_peq.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "gui/components/cymbal_000.hpp"

#include "controllers.hpp"

void controller_mixer::draw(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(name);
    ImGui::PushID(id);
    static log_canvas_000 canvas;
    draw_gui_log_canvas_000("log_canvas", canvas, logger, nullptr);

    static double x = 0.5f;
    static double y = 0.5f;

    static bool show_plot = true;

    static float bar = 0;
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("##bar", &bar, 0.1f, 0, 31, "%.1f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("play")) {
        sqs.play_from_bar(bar);
    }
    ImGui::SameLine();
    if (ImGui::Button("stop")) {
        sqs.stop();
    }
    ImGui::Spacing();

    ImGui::Checkbox("show plot", &show_plot);
    if (show_plot) {
        master_peq.draw();
    }

    draw_mixer_000("new_mix", &graph.mixer_tracks);
    draw_peq_000("##master_peq", graph.main_eq);

    ImGui::PopID();
    ImGui::PopID();
}

void controller_kick::draw(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(name);
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
    ImGui::PopID();
}

void controller_ride::draw(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(name);
    ImGui::PushID(id);

    draw_cymbal_000("ride", graph.ride);
    draw_peq_000("##ride_eq", graph.ride_peq);

    ImGui::PopID();
    ImGui::PopID();
}

void controller_chord::draw(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(name);
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
    ImGui::PopID();
}
