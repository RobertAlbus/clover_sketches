// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "instruments/peq.hpp"
#include <ranges>

#include "imgui-knobs.h"
#include "imgui.h"

#include "peq.hpp"

/*
PEQ TODO

- needs presets
- neither GUI nor Audio thread update the coeffs every tick right now
    - maybe the GUI can own this for now in the prototyping version for simplicity.
*/

void peq_gui(const char* id, peq& peq) {
    ImGui::PushID(id);
    const float knob_size = 30;
    if (ImGui::Button("get peq patch")) {
        ImGui::SetClipboardText(peq.to_str().c_str());
    }
    for (auto [i, these_props] : std::views::zip(std::views::iota(0, int(peq::SIZE)), peq.props)) {
        ImGui::PushID(&these_props);

        // it would be good to use one table for all segments.
        // could also configure this component with a horizontal/vertical flag.
        // this flag would set the number of columns to either 2 or 2*peq::SIZE

        if (ImGui::BeginTable("##table", 2, 0, ImVec2(250, 100))) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (ImGui::Checkbox("enabled", &these_props.enabled)) {
                peq.calculate_coefficients(i);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);

            const char* current_type_str = peq_filter_str[int(these_props.type)];
            ImGui::PushItemWidth(100);
            if (ImGui::BeginCombo("##type", current_type_str)) {
                for (auto [type, str] : std::views::zip(peq_filter_types, peq_filter_str)) {
                    bool is_selected = (type == these_props.type);
                    if (ImGui::Selectable(str, is_selected, 0)) {
                        these_props.type = type;
                        peq.calculate_coefficients(i);
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            ImGui::TableNextColumn();
            if (ImGuiKnobs::Knob(
                        "freq",
                        &these_props.freq,
                        10,
                        24000,
                        0,
                        "%.1f",
                        ImGuiKnobVariant_Tick,
                        knob_size,
                        ImGuiKnobFlags_Logarithmic | ImGuiKnobFlags_AlwaysClamp)) {
                peq.calculate_coefficients(i);
            }
            ImGui::SameLine();
            if (ImGuiKnobs::Knob(
                        "reso",
                        &these_props.reso,
                        0.1,
                        10,
                        0,
                        "%.1f",
                        ImGuiKnobVariant_Tick,
                        knob_size,
                        ImGuiKnobFlags_AlwaysClamp)) {
                peq.calculate_coefficients(i);
            }

            static const std::array<peq_filter_type, 3> needs_gain = {
                    peq_filter_type::hs, peq_filter_type::ls, peq_filter_type::eq};
            if (std::find(needs_gain.cbegin(), needs_gain.cend(), these_props.type) != needs_gain.cend()) {
                ImGui::SameLine();
                if (ImGuiKnobs::Knob(
                            "gain db",
                            &these_props.gain,
                            -24,
                            24,
                            0,
                            "%.1f",
                            ImGuiKnobVariant_Tick,
                            knob_size,
                            ImGuiKnobFlags_AlwaysClamp)) {
                    peq.calculate_coefficients(i);
                }
            }

            ImGui::EndTable();
        }

        ImGui::PopID();
    }
    ImGui::PopID();
}