// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/cymbal/draw_cymbal_000.hpp"
#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/fm/draw_fm_037.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/mixer/draw_mixer_033.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "graph/instrument/draw_driver.hpp"
#include "lib/subtractive_synth/draw_subtractive_synth_036.hpp"

#include "controllers.hpp"

void spacer() {
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
}

void controller_mixer::draw() {
    ImGui::PushID(name);
    ImVec2 meter_dimensions = {20, 200};

    spacer();

    draw_mixer_033("mixer", graph.audio_mixer);

    spacer();

    master_peq.draw();

    ImGui::PopID();
}

void controller_kick::draw() {
    ImGui::PushID(name);

    draw_kick_drum_000("kick_synth", graph.kick);
    spacer();
    gpeq_send.draw();
    spacer();
    draw_fdn8_023_v2("##kick_fdn", graph.kick_verb);
    spacer();
    gpeq_post.draw();

    ImGui::PopID();
}

void controller_bass::draw() {
    ImGui::PushID(name);

    draw_fm_037("##bass_fm", graph.patch.synth.bass_fm_props);

    spacer();

    ImGui::PopID();
}

void controller_snare::draw() {
    ImGui::PushID(name);

    draw_cymbal_000("snare_impulse", graph.snare_body_impulse);

    spacer();

    draw_fdn8_023_v2("##snare_body", graph.snare_body_resonator);
    ImGui::PushID("resonator");
    ImGuiKnobs::Knob("spread", &graph.snare_body_resonator.props.stereo_spread_taps_octaves, 0, 1);

    spacer();

    draw_driver("##snare_body_driver", graph.snare_body_driver);

    ImGui::PopID();

    spacer();

    gpeq.draw();

    spacer();
    spacer();
    ImGui::Text("snare noise");
    draw_subtractive_synth_036("##snare_noise_synth", graph.patch.drums.snare_noise_props);
    spacer();
    gpeq_noise.draw();

    ImGui::PopID();
}

void controller_ride::draw() {
    ImGui::PushID(name);

    draw_cymbal_000("ride", graph.ride);

    spacer();

    gpeq_cymbal.draw();

    ImGui::PopID();
}

void controller_chord::draw() {
    ImGui::PushID(name);
    draw_subtractive_synth_036("##chords", graph.patch.synth.chord_props);

    spacer();

    gpeq_send.draw();

    spacer();

    ImGui::PopID();
}
