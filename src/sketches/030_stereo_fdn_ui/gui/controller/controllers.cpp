// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/cymbal/draw_cymbal_000.hpp"
#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/mixer/draw_mixer.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"

#include "controllers.hpp"

void controller_mixer::draw() {
    ImGui::PushID(name);

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
    draw_fdn8_023_v2("##kick_fdn", graph.kick_verb);
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
    draw_fdn8_023_v2("fdn", graph.chord_verb);
    ImGuiKnobs::Knob("stereo spread", &graph.chord_verb.props.stereo_spread_taps_octaves, -1, 1);
    ImGui::NewLine();
    gpeq_post.draw();
    ImGui::NewLine();

    ImGui::PopID();
}
