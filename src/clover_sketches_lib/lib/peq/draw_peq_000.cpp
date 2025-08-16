// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "peq.hpp"
#include <ranges>

#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/_atom/filter_type.hpp"
#include "peq.hpp"

void draw_peq_000(const char* id, peq_000& peq) {
    ImGui::PushID(id);
    const float knob_size = 30;
    if (ImGui::Button("get peq patch")) {
        ImGui::SetClipboardText(peq.to_str().c_str());
    }
    for (auto [i, these_props] : std::views::zip(std::views::iota(0u, peq_000::SIZE), peq.props)) {
        ImGui::PushID(&these_props);

        if (ImGui::BeginTable("##table", 2, 0, ImVec2(250, 100))) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (ImGui::Checkbox("enabled", &these_props.enabled)) {
                peq.calculate_coefficients(i);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 28);

            filter_type new_type = draw_filter_type_select("##type", these_props.type);
            if (new_type != these_props.type) {
                these_props.type = new_type;
                peq.calculate_coefficients(i);
            }

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

            static const std::array<filter_type, 3> needs_gain = {
                filter_type::hs, filter_type::ls, filter_type::eq};
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
