// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "draw_subtractive_synth_038.hpp"

#include <ranges>

#include "imgui.h"

#include "lib/_atom/filter_type.hpp"
#include "lib/_atom/knob.hpp"
#include "lib/_molecule/adsr/draw_adsr_038.hpp"

bool draw_subtractive_synth_038(const char* id, subtractive_synth_props_038& props) {
    if (ImGui::Button("get synth patch")) {
        ImGui::SetClipboardText(props.to_str().c_str());
    }
    ImGui::PushID(id);
    bool was_changed = false;

    auto origin = ImGui::GetCursorScreenPos();

    ImGui::BeginGroup();
    ImGui::Text("amp");
    was_changed |= draw_adsr_038("##amp_asr", {24000, 24000, 1, 24000}, props.amp_adsr);
    ImGui::Checkbox("retrigger", &props.retrigger);
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text("pitch");
    was_changed |= draw_adsr_038("##pitch_asr", {24000, 24000, 1, 24000}, props.pitch_adsr);

    was_changed |= resettable_knob("tune", &props.tuning, -36, 36, 0, 0, 30);
    ImGui::SameLine();
    was_changed |= resettable_knob("env##tune", &props.pitch_env_octaves, 0, 3, 0, 0, 30);
    ImGui::SameLine();
    was_changed |= resettable_knob("porta", &props.portamento_time, 0, 24000, 0, 0, 30);
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text("cut");
    was_changed |= draw_adsr_038("##cut_asr", {24000, 24000, 1, 24000}, props.cut_adsr);

    was_changed |= resettable_knob("cut", &props.cut, 20, 20000, 1000, 0, 30);
    ImGui::SameLine();
    was_changed |= resettable_knob("env##cut", &props.cut_env_octaves, 0, 4, 0, 0, 30);

    filter_type old_filter_type = props.filter_type;
    props.filter_type           = draw_filter_type_select("##filter_type", props.filter_type);
    was_changed |= old_filter_type != props.filter_type;
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text("resonance");
    was_changed |= draw_adsr_038("##res_asr", {24000, 24000, 1, 24000}, props.res_adsr);

    was_changed |= resettable_knob("res", &props.res, 0.1, 10, 1, 0, 30);
    ImGui::SameLine();
    was_changed |= resettable_knob("env##res", &props.res_env_octaves, 0, 4, 0, 0, 30);
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
