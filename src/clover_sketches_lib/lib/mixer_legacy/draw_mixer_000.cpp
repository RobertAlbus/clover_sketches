// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "draw_mixer_000.hpp"
#include "mixer_000.hpp"

constexpr float MAX_GAIN            = 2;
constexpr ImVec2 channel_dimensions = ImVec2(20, 200);
constexpr ImVec2 cell_padding       = ImVec2(10, 0);

void draw_mixer_000(const char* id, mixer_list_000* mixer) {
    ImGui::PushID(mixer);

    if (ImGui::Button("get mixer patch")) {
        ImGui::SetClipboardText(to_str(*mixer).c_str());
    }
    if (ImGui::BeginTable(
                "mixer",
                int(mixer->size()),
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
        for (auto& mixer_track : *mixer)
            ImGui::TableSetupColumn(mixer_track.name.c_str(), ImGuiTableColumnFlags_AngledHeader);
        ImGui::TableAngledHeadersRow();
        ImGui::PopStyleVar();

        ImGui::TableNextRow();

        for (auto& mixer_track : *mixer) {
            ImGui::TableNextColumn();
            if (mixer_track.spacer) {
                ImGui::Dummy(channel_dimensions);
            } else {
                ImGui::PushID(&mixer_track.gain);
                ImGui::VSliderFloat(
                        "",
                        channel_dimensions,
                        &mixer_track.gain,
                        0,
                        MAX_GAIN,
                        "",
                        ImGuiSliderFlags_NoRoundToFormat);
                ImGui::PopID();
            }
        }

        ImGui::EndTable();
        if (ImGui::Button("zero")) {
            for (auto& mixer_track : *mixer)
                mixer_track.gain = 0;
        }

        ImGui::PopID();
    }
}
