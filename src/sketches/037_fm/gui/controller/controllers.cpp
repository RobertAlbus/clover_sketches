// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/_atom/knob.hpp"
#include "lib/cymbal/draw_cymbal_000.hpp"
#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/fm/draw_fm_037.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/mixer/draw_mixer_033.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "lib/subtractive_synth/draw_subtractive_synth_034.hpp"
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
    ImGui::NewLine();

    ImGui::Text("carrier");
    if (ImGui::Button("get subtractive_synth patch##carrier")) {
        ImGui::SetClipboardText(graph.bass_carrier.to_str().c_str());
    }

    draw_subtractive_synth_034("##bass carrier", graph.patch.synth.bass_carrier);

    ImGui::Text("modulator");
    if (ImGui::Button("get subtractive_synth patch##modulator")) {
        ImGui::SetClipboardText(graph.bass_modulator.to_str().c_str());
    }
    draw_subtractive_synth_034("##bass modulator", graph.patch.synth.bass_modulator);
    ImGui::NewLine();

    resettable_knob("mod depth semitones", &graph.bass_mod_depth_octaves, 0, 3, 0);

    spacer();

    ImGui::PopID();
}

void controller_snare::draw() {
    ImGui::PushID(name);

    draw_cymbal_000("snare_impulse", graph.snare_impulse);

    spacer();

    draw_fdn8_023_v2("##snare_body", graph.snare_resonator);
    ImGui::PushID("resonator");
    ImGuiKnobs::Knob("spread", &graph.snare_resonator.props.stereo_spread_taps_octaves, 0, 1);
    ImGui::PopID();

    spacer();

    if (ImGui::Button("driver preset")) {
        ImGui::SetClipboardText(graph.snare_driver.props.to_str().c_str());
    }
    ImGuiKnobs::Knob("drive_input", &graph.snare_driver.props.drive_input, 0, 8);
    ImGui::SameLine();
    ImGuiKnobs::Knob("drive_clip", &graph.snare_driver.props.drive_clip, 0, 8);
    ImGui::SameLine();
    ImGuiKnobs::Knob("clip_thresh", &graph.snare_driver.props.clip_thresh, 0, 2);
    ImGui::SameLine();
    ImGuiKnobs::Knob("trim", &graph.snare_driver.props.trim, 0, 8);
    ImGui::SameLine();

    ImGui::PopID();

    spacer();

    gpeq.draw();

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
    if (ImGui::Button("get subtractive_synth patch")) {
        ImGui::SetClipboardText(graph.chord[0].props.to_str().c_str());
    }

    draw_subtractive_synth_036("##chords", graph.patch.synth.chord_props);

    spacer();

    gpeq_send.draw();

    spacer();

    ImGui::PopID();
}
