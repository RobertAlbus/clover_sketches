// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/logging/draw_gui_log_canvas.hpp"
#include "lib/mixer/draw_mixer.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "gui/components/cymbal_000.hpp"

#include "controllers.hpp"

void controller_mixer::draw() {
    ImGui::PushID(name);
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
    ImGui::NewLine();

    ImGui::NewLine();
    draw_mixer_000("new_mix", &graph.mixer_tracks);
    ImGui::NewLine();
    master_peq.draw();
    ImGui::NewLine();

    ImGui::PopID();
}

void controller_kick::draw() {
    ImGui::PushID(name);

    draw_kick_drum_000("kick_synth", graph.kick);
    ImGui::NewLine();
    gpeq_send.draw();
    ImGui::NewLine();
    draw_fdn8_023("##kick_fdn", &graph.kick_verb, nullptr);
    ImGui::NewLine();
    gpeq_post.draw();
    ImGui::NewLine();

    ImGui::PopID();
}

void controller_ride::draw() {
    ImGui::PushID(name);

    draw_cymbal_000("ride", graph.ride);
    ImGui::NewLine();
    gpeq_cymbal.draw();
    ImGui::NewLine();

    ImGui::PopID();
}

void controller_chord::draw() {
    ImGui::PushID(name);

    gpeq_send.draw();
    ImGui::NewLine();
    draw_fdn8_023("fdn", &graph.chord_verb_L, &graph.chord_verb_R);
    ImGui::NewLine();
    gpeq_post.draw();
    ImGui::NewLine();

    ImGui::PopID();
}
