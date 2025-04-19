// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "composition/mix.hpp"

#include "mixer_ui.hpp"

const float MAX_GAIN            = 2;
const ImVec2 channel_dimensions = ImVec2(20, 200);
const ImVec2 cell_padding       = ImVec2(10, 0);

void mixer_component(const char* id, std::vector<mixer_track>* mixer_tracks) {
    ImGui::PushID(mixer_tracks);

    if (ImGui::Button("get mixer patch")) {
        ImGui::SetClipboardText(to_str(*mixer_tracks).c_str());
    }
    if (ImGui::BeginTable(
                "mixer",
                int(mixer_tracks->size()),
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
        for (auto& mixer_track : *mixer_tracks)
            ImGui::TableSetupColumn(mixer_track.name.c_str(), ImGuiTableColumnFlags_AngledHeader);
        ImGui::TableAngledHeadersRow();
        ImGui::PopStyleVar();

        ImGui::TableNextRow();

        for (auto& mixer_track : *mixer_tracks) {
            ImGui::PushID(&mixer_track.gain);
            ImGui::TableNextColumn();
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

        ImGui::EndTable();
        if (ImGui::Button("zero")) {
            for (auto& mixer_track : *mixer_tracks)
                mixer_track.gain = 0;
        }

        ImGui::PopID();
    }
}

// struct mixer_channel_ui {
//     const char* id;
//     float* mix;
// };
// void mixer_component(const char* id, composition_mix* mixer) {
//     const std::array<mixer_channel_ui, 7> mixer_channels{
//             mixer_channel_ui{"kick_send", &mixer->kick_send},
//             mixer_channel_ui{"kick_wet", &mixer->kick_wet},
//             mixer_channel_ui{"kick_gain", &mixer->kick_gain},

//             mixer_channel_ui{"chord_send", &mixer->chord_send},
//             mixer_channel_ui{"chord_wet", &mixer->chord_wet},
//             mixer_channel_ui{"chord_dry", &mixer->chord_dry},
//             mixer_channel_ui{"chord_sum", &mixer->chord_sum},

//     };

//     if (ImGui::Button("zero")) {
//         for (auto& mixer_channel : mixer_channels)
//             *(mixer_channel.mix) = 0;
//     }
//     if (ImGui::BeginTable(
//                 "mixer",
//                 int(mixer_channels.size()),
//                 ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
//         ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
//         for (auto& mixer_channel : mixer_channels)
//             ImGui::TableSetupColumn(mixer_channel.id, ImGuiTableColumnFlags_AngledHeader);
//         ImGui::TableAngledHeadersRow();
//         ImGui::PopStyleVar();

//         ImGui::TableNextRow();

//         for (auto& mixer_channel : mixer_channels) {
//             ImGui::PushID(mixer_channel.mix);
//             ImGui::TableNextColumn();
//             ImGui::VSliderFloat(
//                     "",
//                     channel_dimensions,
//                     mixer_channel.mix,
//                     0,
//                     MAX_GAIN,
//                     "",
//                     ImGuiSliderFlags_NoRoundToFormat);
//             ImGui::PopID();
//         }

//         ImGui::EndTable();

//         if (ImGui::Button("get mixer patch")) {
//             ImGui::SetClipboardText(mixer->to_str().c_str());
//         }
//     }
// }
