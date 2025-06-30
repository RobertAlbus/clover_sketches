// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "draw_mixer.hpp"
#include "lib/meter/draw_meter.hpp"
#include "mixer.hpp"

constexpr float MAX_GAIN            = 2;
constexpr ImVec2 dimensions_channel = ImVec2(40, 200);
constexpr ImVec2 dimensions_spacer  = ImVec2(dimensions_channel.x / 2, dimensions_channel.y);
constexpr ImVec2 cell_padding       = ImVec2(10, 0);

bool draw_pan_knob(float* pan_v) {
    constexpr float pan_L_angle = std::numbers::pi - 0.5;
    constexpr float pan_R_angle = (2 * std::numbers::pi) + 0.5;
    ImGui::PushID("##pan_knob");
    bool was_changed = ImGuiKnobs::Knob(
            "",
            pan_v,
            -1,
            1,
            0,
            "",
            ImGuiKnobVariant_Dot,
            dimensions_channel.x,
            ImGuiKnobFlags_NoInput,
            -1,
            pan_L_angle,
            pan_R_angle);
    if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
        *pan_v      = 0;
        was_changed = true;
    }
    ImGui::PopID();

    return was_changed;
}

bool draw_red_button(const char* id, const bool* active) {
    const ImU32 color_red     = ImColor(255, 0, 0, 200);
    const ImU32 color_red_dim = ImColor(255, 0, 0, 100);
    const ImU32 color_none    = ImColor(0, 0, 0, 0);

    auto set_colors = [&](bool is_muted) {
        const ImU32 color_bg      = *active ? color_red : color_none;
        const ImU32 color_hovered = *active ? color_red : color_red_dim;
        const ImU32 color_active  = color_red;

        ImGui::PushStyleColor(ImGuiCol_Button, color_bg);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active);
    };

    set_colors(*active);
    bool touched = ImGui::Button(id, {dimensions_channel.x, dimensions_channel.x});
    if (touched) {
        ImGui::PopStyleColor(3);
        set_colors(*active);
    }

    ImGui::PopStyleColor(3);
    return touched;
}

void draw_mixer_033(const char* id, mixer_033& mixer) {
    ImGui::PushID(id);
    ImGui::PushID(&mixer);

    if (ImGui::Button("get mixer patch")) {
        ImGui::SetClipboardText(mixer.to_str().c_str());
    }
    if (ImGui::BeginTable(
                "mixer",
                int(mixer.channels.size() + 1),
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

                draw_meter(dimensions_channel, strip.meter);
                ImGui::NewLine();

                if (draw_pan_knob(&strip.props.pan)) {
                    strip.update_pan();
                }
                if (draw_red_button("mono", &strip.props.mono)) {
                    strip.props.mono = !strip.props.mono;
                }

                ImGui::VSliderFloat(
                        "",
                        dimensions_channel,
                        &strip.props.gain,
                        0,
                        MAX_GAIN,
                        "",
                        ImGuiSliderFlags_NoRoundToFormat);
                if (draw_red_button("mute", &strip.props.mute)) {
                    strip.props.mute = !strip.props.mute;
                }
                ImGui::PopID();
            } else {
                ImGui::Dummy(dimensions_spacer);
            }
        }
        ImGui::TableNextColumn();
        ImGui::Dummy(dimensions_spacer);

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
