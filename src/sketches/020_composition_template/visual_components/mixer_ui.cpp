// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>

#include "imgui.h"

#include "composition/mix.hpp"

#include "mixer_ui.hpp"

struct mixer_channel_ui {
    const char* id;
    float* mix;
};

const float MAX_GAIN            = 2;
const ImVec2 channel_dimensions = ImVec2(20, 200);
const ImVec2 cell_padding       = ImVec2(10, 0);
void mixer_component(const char* id, composition_mix* mixer) {
    const std::array<mixer_channel_ui, 26> mixer_channels{
            mixer_channel_ui{"kick_send", &mixer->kick_send},
            mixer_channel_ui{"kick_wet", &mixer->kick_wet},
            mixer_channel_ui{"kick_gain", &mixer->kick_gain},
            mixer_channel_ui{"bass_gain", &mixer->bass_gain},

            mixer_channel_ui{"hh1", &mixer->hh1},
            mixer_channel_ui{"hh2", &mixer->hh2},
            mixer_channel_ui{"hh3", &mixer->hh3},
            mixer_channel_ui{"hh_verb_send", &mixer->hh_verb_send},
            mixer_channel_ui{"hh_verb_wet", &mixer->hh_verb_wet},
            mixer_channel_ui{"hh_bus", &mixer->hh_bus},
            mixer_channel_ui{"hh_drive", &mixer->hh_drive},
            mixer_channel_ui{"hh_trim", &mixer->hh_trim},
            mixer_channel_ui{"ride", &mixer->ride},
            mixer_channel_ui{"cymbal_bus", &mixer->cymbal_bus},

            mixer_channel_ui{"chord_send", &mixer->chord_send},
            mixer_channel_ui{"chord_wet", &mixer->chord_wet},
            mixer_channel_ui{"chord_dry", &mixer->chord_dry},
            mixer_channel_ui{"chord_sum", &mixer->chord_sum},

            mixer_channel_ui{"pad_send", &mixer->pad_send},
            mixer_channel_ui{"pad_wet", &mixer->pad_wet},
            mixer_channel_ui{"pad_dry", &mixer->pad_dry},
            mixer_channel_ui{"pad_sum", &mixer->pad_sum},

            mixer_channel_ui{"lead_a", &mixer->lead_a},
            mixer_channel_ui{"lead_b", &mixer->lead_b},
            mixer_channel_ui{"lead_ringmod", &mixer->lead_ringmod},
            mixer_channel_ui{"lead", &mixer->lead},
    };

    if (ImGui::Button("zero")) {
        for (auto& mixer_channel : mixer_channels)
            *(mixer_channel.mix) = 0;
    }
    if (ImGui::BeginTable(
                "mixer",
                int(mixer_channels.size()),
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
        for (auto& mixer_channel : mixer_channels)
            ImGui::TableSetupColumn(mixer_channel.id, ImGuiTableColumnFlags_AngledHeader);
        ImGui::TableAngledHeadersRow();
        ImGui::PopStyleVar();

        ImGui::TableNextRow();

        for (auto& mixer_channel : mixer_channels) {
            ImGui::PushID(mixer_channel.mix);
            ImGui::TableNextColumn();
            ImGui::VSliderFloat(
                    "",
                    channel_dimensions,
                    mixer_channel.mix,
                    0,
                    MAX_GAIN,
                    "",
                    ImGuiSliderFlags_NoRoundToFormat);
            ImGui::PopID();
        }

        ImGui::EndTable();

        if (ImGui::Button("get mixer patch")) {
            ImGui::SetClipboardText(mixer->to_str().c_str());
        }
    }
}
