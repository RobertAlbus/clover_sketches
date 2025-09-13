// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/cymbal/draw_cymbal_038.hpp"
#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/fm/draw_fm_037.hpp"
#include "lib/kick_drum/draw_kick_drum_038.hpp"
#include "lib/mixer/draw_mixer_033.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "graph/instrument/draw_driver.hpp"
#include "lib/subtractive_synth/draw_subtractive_synth_038.hpp"

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

    draw_kick_drum_038("kick_synth", graph.kick);
    spacer();
    gpeq_send.draw();
    spacer();
    draw_fdn8_023_v2("##kick_fdn", graph.kick_verb);
    spacer();
    gpeq_return.draw();
    spacer();
    gpeq_post.draw();

    ImGui::PopID();
}

void controller_bass::draw() {
    ImGui::PushID(name);

    draw_fm_037("##bass_fm", graph.patch.synth.bass_fm_props);

    spacer();
    gpeq_send.draw();
    spacer();
    ImGuiKnobs::Knob("stereo spread", &graph.bass_verb.props.stereo_spread_taps_octaves, -1, 1);
    draw_fdn8_023_v2("##bass_fdn", graph.bass_verb);
    spacer();
    gpeq_return.draw();
    spacer();
    gpeq_post.draw();

    ImGui::PopID();
}

void controller_snare::draw() {
    ImGui::PushID(name);

    draw_cymbal_038("snare_impulse", graph.snare_body_impulse);

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
    draw_subtractive_synth_038("##snare_noise_synth", graph.patch.drums.snare_noise_props);
    spacer();
    gpeq_noise.draw();

    ImGui::PopID();
}

void controller_ride::draw() {
    ImGui::PushID(name);

    draw_cymbal_038("ride", graph.ride);
    gpeq_cymbal.draw();

    spacer();

    ImGui::Text("hh1");
    draw_cymbal_038("hh1", graph.hh1);
    draw_driver("##hh1_driver", graph.hh1_driver);
    gpeq_hh1.draw();

    spacer();

    ImGui::Text("hh2");
    draw_cymbal_038("hh2", graph.hh2);
    draw_driver("##hh2_driver", graph.hh2_driver);
    gpeq_hh2.draw();

    spacer();

    ImGui::Text("hh3");
    draw_cymbal_038("hh3", graph.hh3);
    gpeq_hh3.draw();

    spacer();

    ImGui::PopID();
}

void controller_chord::draw() {
    ImGui::PushID(name);
    draw_subtractive_synth_038("##chords", graph.patch.synth.chord_props);

    spacer();

    draw_fdn8_023_v2("fdn", graph.chord_verb);
    ImGuiKnobs::Knob("stereo spread", &graph.chord_verb.props.stereo_spread_taps_octaves, -1, 1);

    spacer();

    gpeq_send.draw();

    ImGui::PopID();
}
