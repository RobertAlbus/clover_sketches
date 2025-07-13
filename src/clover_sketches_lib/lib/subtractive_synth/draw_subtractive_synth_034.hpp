#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "imgui.h"

#include "lib/_atom/knob.hpp"
#include "lib/_component/adsr/draw_adsr.hpp"

#include "lib/subtractive_synth/filter_block_000.hpp"
#include "lib/subtractive_synth/nx_osc_034.hpp"
#include "lib/subtractive_synth/subtractive_synth_034.hpp"

bool draw_nx_osc(const char* id, nx_osc_props_034& props) {
    ImGui::PushID(id);
    bool was_changed = false;

    auto origin = ImGui::GetCursorScreenPos();

    ImGui::BeginGroup();
    ImGui::Text("amp");
    was_changed |= draw_adsr_000(
        "##amp_asr", {24000, 24000, 1, 24000}, props.amp_a, props.amp_d, props.amp_s, props.amp_r);
    ImGui::Checkbox("retrigger", &props.retrigger);
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text("pitch");
    was_changed |= draw_adsr_000(
        "##amp_asr", {24000, 24000, 1, 24000}, props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);

    was_changed |= resettable_knob("tuning", &props.tuning, -36, 36, 0, 0, 30);
    ImGui::SameLine();
    was_changed |= resettable_knob("porta", &props.portamento_time, 0, 24000, 0, 0, 30);
    ImGui::SameLine();
    was_changed |= resettable_knob("pitch env", &props.pitch_env_octaves, 0, 3, 0, 0, 30);
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Dummy({50, 100});
    ImGui::SameLine();

    ImGui::BeginGroup();
    const auto text_wave            = "wave";
    const auto text_gain            = "gain";
    const auto text_semi            = "semitone";
    const auto text_pan             = "pan";
    const float knob_label_width    = ImGui::CalcTextSize(text_semi).x;
    const float label_section_width = knob_label_width + ImGui::GetStyle().ItemSpacing.x;
    const float right_x             = ImGui::GetCursorPosX() + knob_label_width;
    ImGui::SetCursorPosX(right_x - ImGui::CalcTextSize(text_wave).x);
    ImGui::Text(text_wave);
    ImGui::Dummy({label_section_width, 55});
    ImGui::SetCursorPosX(right_x - ImGui::CalcTextSize(text_gain).x);
    ImGui::Text(text_gain);
    ImGui::Dummy({label_section_width, 60});
    ImGui::SetCursorPosX(right_x - ImGui::CalcTextSize(text_semi).x);
    ImGui::Text(text_semi);
    ImGui::Dummy({label_section_width, 60});
    ImGui::SetCursorPosX(right_x - ImGui::CalcTextSize(text_pan).x);
    ImGui::Text(text_pan);
    ImGui::EndGroup();

    ImGui::SameLine();

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    auto osc_zip = std::views::zip(
        props.osc_gains,
        props.osc_pans,
        props.osc_tunings,
        props.waveforms,
        std::views::iota(0u, props.waveforms.size()));

    const float osc_column_width = 50;

    for (auto [gain, pan, tuning, osc_waveform, i] : osc_zip) {
        ImGui::PushID(int(i));

        ImGui::SameLine();
        ImGui::BeginGroup();

        ImGui::PushItemWidth(osc_column_width);
        if (ImGui::BeginCombo("##type", waveform_to_str(osc_waveform), ImGuiComboFlags_NoArrowButton)) {
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

        was_changed |= resettable_knob("##gain", &gain, -1, 1, 0);
        was_changed |= resettable_knob("##semi", &tuning, -36, 36, 0);
        was_changed |= resettable_knob("##pan", &pan, -1, 1, 0);

        ImGui::EndGroup();
        ImGui::PopID();
    }

    ImGui::PopID();
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