// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "draw_mixer.hpp"
#include "lib/meter/draw_meter.hpp"
#include "mixer.hpp"

constexpr float MAX_GAIN            = 2;
constexpr ImVec2 channel_dimensions = ImVec2(20, 200);
constexpr ImVec2 cell_padding       = ImVec2(10, 0);

void draw_mixer_033(const char* id, mixer_033& mixer) {
    ImGui::PushID(id);
    ImGui::PushID(&mixer);

    if (ImGui::Button("get mixer patch")) {
        ImGui::SetClipboardText(mixer.to_str().c_str());
    }
    if (ImGui::BeginTable(
                "mixer",
                int(mixer.channels.size()),
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
        for (auto& channel : mixer.channels)
            ImGui::TableSetupColumn(channel.spec.name.c_str(), ImGuiTableColumnFlags_AngledHeader);
        ImGui::TableAngledHeadersRow();
        ImGui::PopStyleVar();

        ImGui::TableNextRow();

        for (auto& channel : mixer.channels) {
            ImGui::TableNextColumn();
            if (channel.strip.has_value()) {
                channel_strip& strip = channel.strip.value();

                ImGui::PushID(&strip.props.gain);

                draw_meter(channel_dimensions, strip.meter);
                ImGui::NewLine();
                ImGui::VSliderFloat(
                        "",
                        channel_dimensions,
                        &strip.props.gain,
                        0,
                        MAX_GAIN,
                        "",
                        ImGuiSliderFlags_NoRoundToFormat);
                ImGui::PopID();
            } else {
                ImGui::Dummy(channel_dimensions);
            }
        }

        ImGui::EndTable();
        if (ImGui::Button("zero")) {
            for (auto& channel : mixer.channels)
                if (channel.strip.has_value())
                    channel.strip.value().props.gain = 0;
        }

        ImGui::PopID();
        ImGui::PopID();
    }
}
