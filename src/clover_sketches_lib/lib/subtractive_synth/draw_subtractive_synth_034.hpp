#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "imgui-knobs.h"
#include "imgui.h"
#include "lib/_component/adsr/draw_adsr.hpp"

#include "lib/subtractive_synth/filter_block_000.hpp"
#include "lib/subtractive_synth/nx_osc_034.hpp"
#include "lib/subtractive_synth/subtractive_synth_034.hpp"

bool draw_nx_osc(const char* id, nx_osc_props_034& props) {
    ImGui::PushID(id);
    bool was_changed = false;

    auto origin = ImGui::GetCursorScreenPos();

    ImGui::Text("amp");
    was_changed |= draw_adsr_000(
        "##amp_asr", {48000, 48000, 1, 48000}, props.amp_a, props.amp_d, props.amp_s, props.amp_r);

    ImGui::NewLine();

    ImGui::Text("pitch");
    was_changed |= draw_adsr_000(
        "##amp_asr", {48000, 48000, 1, 48000}, props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);

    ImGui::SameLine();
    auto adsr_end_pos    = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGui::NewLine();

    ImGuiKnobs::Knob("pitch octaves", &props.pitch_env_octaves, -3, 3);

    ImGui::NewLine();
    auto end_position = ImGui::GetCursorScreenPos();

    ImGui::SetCursorScreenPos({adsr_end_pos.x, origin.y});

    auto osc_zip = std::views::zip(
        props.osc_gains,
        props.osc_pans,
        props.osc_tunings,
        props.waveforms,
        std::views::iota(0u, props.waveforms.size()));

    for (auto [gain, pan, tuning, osc_waveform, i] : osc_zip) {
        ImGui::PushID(int(i));

        ImGui::SetCursorPosX(adsr_end_pos.x);
        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##type", waveform_to_str(osc_waveform))) {
            for (auto [waveform, str] : std::views::zip(waveform_list_000, waveform_str_000)) {
                bool is_selected = (waveform == osc_waveform);
                if (ImGui::Selectable(str, is_selected, 0)) {
                    osc_waveform = waveform;
                    was_changed  = true;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGuiKnobs::Knob("gain", &gain, -1, 1)) {
            was_changed = true;
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
            gain        = 0;
            was_changed = true;
        }

        ImGui::SameLine();
        if (ImGuiKnobs::Knob("semi", &tuning, -36, 36)) {
            was_changed = true;
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
            tuning      = 0;
            was_changed = true;
        }

        ImGui::SameLine();
        if (ImGuiKnobs::Knob("pan", &pan, -1, 1)) {
            was_changed = true;
        }
        if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
            pan         = 0;
            was_changed = true;
        }

        ImGui::PopID();
    }

    ImGui::PopID();
    ImGui::SetCursorScreenPos(end_position);
    return was_changed;
}

bool draw_filter_block(const char* id, filter_block_props_000& osc_block) {
    return false;
}

bool draw_subtractive_synth_034(const char* id, subtractive_synth_props_034& props) {
    bool oscs_touched    = draw_nx_osc(id, props.osc_props);
    bool filters_touched = draw_filter_block(id, props.filter_props);

    return oscs_touched | filters_touched;
}