// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "graph/instrument/meter.hpp"
#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/cymbal/draw_cymbal_000.hpp"
#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/mixer/draw_mixer.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"

#include "controllers.hpp"

void spacer() {
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
}

void controller_mixer::draw() {
    ImGui::PushID(name);
    ImVec2 meter_dimensions = {10, 200};
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    draw_meter(  //
            meter_dimensions,
            graph.main_meter_L.peak,
            graph.main_meter_L.peak_held,
            graph.main_meter_L.rms);
    ImGui::SameLine();
    draw_meter(  //
            meter_dimensions,
            graph.main_meter_R.peak,
            graph.main_meter_R.peak_held,
            graph.main_meter_R.rms);
    ImGui::PopStyleVar();

    spacer();

    draw_mixer_000("new_mix", &graph.mixer_tracks);

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

    spacer();

    draw_fdn8_023_v2("##snare_verb", graph.snare_verb);
    ImGui::PushID("verb");
    ImGuiKnobs::Knob("spread", &graph.snare_verb.props.stereo_spread_taps_octaves, 0, 1);
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

    gpeq_send.draw();

    spacer();

    draw_fdn8_023_v2("fdn", graph.chord_verb);
    ImGuiKnobs::Knob("stereo spread", &graph.chord_verb.props.stereo_spread_taps_octaves, -1, 1);

    spacer();

    gpeq_post.draw();

    ImGui::PopID();
}
