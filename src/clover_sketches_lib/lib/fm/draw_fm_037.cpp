// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/math.hpp"
#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/_atom/filter_type.hpp"
#include "lib/_atom/resettable.hpp"
#include "lib/_molecule/adsr/draw_adsr_038.hpp"
#include <format>
#include <ranges>
#include <string>

#include "draw_fm_037.hpp"

namespace {
const float padding_size = 30;
const float knob_size    = 50;
}  // namespace

void style_knob_faded() {
    ImVec4 color{};
    color   = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
    color.w = 0.5;
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    color   = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
    color.w = 0.5;
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    color   = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    color.w = 0.5;
    ImGui::PushStyleColor(ImGuiCol_Button, color);
}
void style_knob_faded_end() {
    ImGui::PopStyleColor(3);
}

bool draw_fm_037(const char* id, fm_props_037& props) {
    ImGuiKnobFlags knob_flags = 0;
    ImGui::PushID(id);
    if (ImGui::Button("get fm patch")) {
        ImGui::SetClipboardText(props.to_str().c_str());
    }
    ImGui::NewLine();
    bool was_changed = false;

    // Left side: Operators
    ImGui::BeginGroup();
    for (auto op : std::views::iota(0, 6)) {
        ImGui::PushID(op);

        ImGui::BeginGroup();
        ImGui::PushItemWidth(100);
        ImGui::Text("Op %d", op + 1);

        bool is_ratio = props.tuning_types[op] == fm_tuning_type::ratio;
        bool is_hz    = props.tuning_types[op] == fm_tuning_type::hz;
        if (ImGui::RadioButton("ratio", is_ratio)) {
            props.tuning_types[op] = fm_tuning_type::ratio;
            was_changed            = true;
        }
        ImGui::SameLine();
        float max_tuning   = is_ratio ? 16 : 20000;
        bool is_shift_held = ImGui::IsKeyDown(ImGuiKey_LeftShift);
        float drag_speed   = 1;
        if (is_ratio) {
            drag_speed = is_shift_held ? 0.01f : 1.f;
        } else {
            drag_speed = is_shift_held ? props.tunings[op] * 0.01f : props.tunings[op] * 0.1f;
        }
        was_changed |= ImGui::DragFloat(
            "##tuning",
            &props.tunings[op],
            drag_speed,
            0,
            max_tuning,
            "%.2f",
            ImGuiSliderFlags_NoSpeedTweaks);
        if (ImGui::RadioButton("hz", is_hz)) {
            props.tuning_types[op] = fm_tuning_type::hz;
            was_changed            = true;
        }
        ImGui::PopItemWidth();
        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text("amp");
        draw_adsr_038(
            "##amp_adsr", {24000, 24000, 1, 24000}, props.amp_adsrs[op], 200, 50, knob_size, knob_flags);
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::Dummy({padding_size / 2, 0});  // amp right padding
        ImGui::SameLine();

        ImGui::BeginGroup();
        ImGui::Text("pitch");
        was_changed |= draw_adsr_038(
            "##pitch_adsr", {24000, 24000, 1, 24000}, props.pitch_adsrs[op], 200, 50, knob_size, knob_flags);

        ImGui::SameLine();
        ImGui::PushID("mod");
        was_changed |= ImGuiKnobs::Knob(
            "",
            &props.pitch_env_mod_depths[op],
            0,
            4,
            0,
            "%.3f",
            ImGuiKnobVariant_Tick,
            knob_size,
            knob_flags);
        was_changed |= resettable(props.pitch_env_mod_depths[op], 0);
        ImGui::PopID();
        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::Dummy({padding_size, 0});  // mod matrix left padding
        ImGui::PopID();
    }
    ImGui::EndGroup();

    ImGui::SameLine();

    // Center: Modulation Matrix
    ImGui::BeginGroup();

    ImGui::BeginGroup();
    for (auto target : std::views::iota(0, 6)) {
        // if (target > 0)
        //     ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 12);
        for (auto source : std::views::iota(0, 6)) {
            int matrix_index = target * 6 + source;
            bool set_to_zero = props.mod_matrix[matrix_index] == 0;
            if (set_to_zero) {
                style_knob_faded();
            }

            ImGui::PushID(matrix_index);
            was_changed |= ImGuiKnobs::Knob(
                "",
                &props.mod_matrix[matrix_index],
                -1.0f,
                1.0f,
                0.0f,
                "%.2f",
                ImGuiKnobVariant_Tick,
                knob_size,
                ImGuiKnobFlags_AlwaysClamp | knob_flags | ImGuiKnobFlags_NoTitle);
            was_changed |= resettable(props.mod_matrix[matrix_index], 0);
            ImGui::PopID();

            if (set_to_zero) {
                style_knob_faded_end();
            }
            if (source < 5)
                ImGui::SameLine();
        }
    }
    ImGui::SameLine();
    ImGui::Dummy({padding_size, 0});  // mod matrix right padding

    ImGui::Dummy({0, padding_size});  // mod matrix bottom padding

    ImGui::BeginGroup();
    ImGui::PushID("gain");
    for (auto [op, gain, pan] :
         std::views::zip(std::views::iota(0, 6), props.op_output_gains, props.op_pans)) {
        ImGui::PushID(op);
        bool set_to_zero = gain == 0;
        if (set_to_zero) {
            style_knob_faded();
        }
        ImGuiKnobs::Knob(
            "",
            &gain,
            -1,
            1,
            0,
            "%.2f",
            ImGuiKnobVariant_Tick,
            knob_size,
            knob_flags | ImGuiKnobFlags_NoTitle);
        if (set_to_zero) {
            style_knob_faded_end();
        }
        ImGui::PopID();
        was_changed |= resettable(gain, 0);
        ImGui::SameLine();
    }
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::PushID("pan");
    for (auto [op, gain, pan] :
         std::views::zip(std::views::iota(0, 6), props.op_output_gains, props.op_pans)) {
        ImGui::PushID(op);
        bool set_to_zero = pan == 0;
        if (set_to_zero) {
            style_knob_faded();
        }
        ImGuiKnobs::Knob(
            "",
            &pan,
            -1,
            1,
            0,
            "%.2f",
            ImGuiKnobVariant_Tick,
            knob_size,
            knob_flags | ImGuiKnobFlags_NoTitle);
        if (set_to_zero) {
            style_knob_faded_end();
        }
        was_changed |= resettable(pan, 0);
        ImGui::PopID();

        ImGui::SameLine();
    }
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::EndGroup();

    // Right side
    ImGui::SameLine();
    ImGui::BeginGroup();

    // various controls
    ImGui::BeginGroup();
    ImGui::BeginGroup();
    ImGui::PushID("fm_depth");
    ImGui::Text("fm depth");
    was_changed |= ImGuiKnobs::Knob(
        "",
        &props.matrix_octave_range,
        0,
        3,
        0,
        "%.2f",
        ImGuiKnobVariant_Tick,
        knob_size,
        knob_flags | ImGuiKnobFlags_NoTitle);
    was_changed |= resettable(props.matrix_octave_range, 3);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::Text("retrigger");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Checkbox("##retrigger", &props.retrigger);
    ImGui::EndGroup();
    ImGui::EndGroup();

    // filter
    ImGui::Dummy({0, padding_size});  // filter top padding
    ImGui::BeginGroup();

    ImGui::BeginGroup();
    ImGui::Dummy({0, knob_size});
    ImGui::Text("Filter");
    filter_type old_filter_type = props.filter_type;
    props.filter_type           = draw_filter_type_select("##filter_type", props.filter_type);
    was_changed |= old_filter_type != props.filter_type;
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::PushID("cut");
    float cut_octaves = clover::octave_difference_by_frequency(20.0f, props.cut);
    float max_octaves = clover::octave_difference_by_frequency(20.0f, 20000.0f);
    if (ImGuiKnobs::Knob(
            "cut",
            &cut_octaves,
            0.0f,
            max_octaves,
            0,
            std::format("{:.0f}", props.cut).c_str(),
            ImGuiKnobVariant_Tick,
            knob_size,
            knob_flags)) {
        props.cut   = clover::frequency_by_octave_difference(20.0f, cut_octaves);
        was_changed = true;
    }
    was_changed |= resettable(props.cut, 20000);
    ImGui::PopID();

    ImGui::SameLine();
    ImGui::PushID("res");
    was_changed |= ImGuiKnobs::Knob(
        "reso", &props.res, 0.1f, 10.0f, 0, "%.3f", ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(props.res, 0.707f);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::Dummy({0, padding_size});  // cut env top padding

    ImGui::BeginGroup();
    ImGui::Text("cut env");
    was_changed |=
        draw_adsr_038("##cut_adsr", {24000, 24000, 1, 24000}, props.cut_adsr, 200, 50, knob_size, knob_flags);
    ImGui::PushID("cut_mod_target");
    float cut_mod_target_octaves = clover::octave_difference_by_frequency(20.0f, props.cut_mod_target);
    ImGui::SameLine();
    if (ImGuiKnobs::Knob(
            "",
            &cut_mod_target_octaves,
            0.0f,
            max_octaves,
            0,
            std::format("{:.0f}", props.cut_mod_target).c_str(),
            ImGuiKnobVariant_Tick,
            knob_size,
            knob_flags)) {
        props.cut_mod_target = clover::frequency_by_octave_difference(20.0f, cut_mod_target_octaves);
        was_changed          = true;
    }
    was_changed |= resettable(props.cut_mod_target, 20000);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::Dummy({0, padding_size});  // res env top padding

    ImGui::BeginGroup();
    ImGui::Text("res env");

    was_changed |=
        draw_adsr_038("##res_adsr", {24000, 24000, 1, 24000}, props.res_adsr, 200, 50, knob_size, knob_flags);
    ImGui::SameLine();
    ImGui::PushID("res_mod_target");
    was_changed |= ImGuiKnobs::Knob(
        "", &props.res_mod_target, 0.1f, 10.0f, 0, "%.3f", ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(props.res_mod_target, 0.707f);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::EndGroup();
    ImGui::EndGroup();

    ImGui::PopID();
    return was_changed;
}
