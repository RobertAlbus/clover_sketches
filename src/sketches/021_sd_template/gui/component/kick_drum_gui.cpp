// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "adsr.hpp"

#include "kick_drum_gui.hpp"

void kick_drum_gui(const char* id, kick_drum& kick) {
    ImGui::PushID(id);

    if (ImGui::Button("get kick patch")) {
        ImGui::SetClipboardText(kick.props.to_str().c_str());
    }

    if (ImGui::BeginTable("##table", 4)) {
        ImGui::TableSetupColumn("amp", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("pitch", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("saturation", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("filter", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        if (adsr("##amp_asr",
                 {1000, 40000, 1, 1000},
                 kick.props.amp_a,
                 kick.props.amp_d,
                 kick.props.amp_s,
                 kick.props.amp_r)) {
            kick.adsr_amp.set(kick.props.amp_a, kick.props.amp_d, kick.props.amp_s, kick.props.amp_r);
        }
        ImGui::TableNextColumn();
        if (adsr("##pitch_asr",
                 {1000, 10000, 1, 1000},
                 kick.props.pitch_a,
                 kick.props.pitch_d,
                 kick.props.pitch_s,
                 kick.props.pitch_r)) {
            kick.adsr_pitch.set(
                    kick.props.pitch_a, kick.props.pitch_d, kick.props.pitch_s, kick.props.pitch_r);
        }
        ImGui::PushItemWidth(65);
        ImGui::DragFloat(
                "mod range##pitch", &kick.props.pitch_range, kick.props.pitch_range * 0.1f, 0, 8, "%.2f");
        ImGui::DragFloat(
                "fundamental##pitch",
                &kick.props.pitch_fundamental,
                kick.props.pitch_fundamental * 0.1f,
                0,
                10,
                "%.0f");

        ImGui::PopItemWidth();
        ImGui::TableNextColumn();

        float drive_gain_min = 0;
        float drive_gain_max = 10;
        float trim_gain_min  = 0;
        float trim_gain_max  = 1;

        ImGui::VSliderFloat(
                "##drive_slider", ImVec2(30, 100), &kick.props.drive, drive_gain_min, drive_gain_max);
        ImGui::SameLine();
        ImGui::VSliderFloat("##trim_slider", ImVec2(30, 100), &kick.props.trim, trim_gain_min, trim_gain_max);

        ImGui::PushItemWidth(30);

        ImGui::DragFloat(
                "##drive_drag", &kick.props.drive, 0.2f * kick.props.trim, drive_gain_min, drive_gain_max);
        ImGui::SameLine();
        ImGui::DragFloat(
                "##trim_drag", &kick.props.trim, 0.2f * kick.props.trim, trim_gain_min, trim_gain_max);

        ImGui::PopItemWidth();

        ImGui::TableNextColumn();
        if (adsr("##cut_asr",
                 {1000, 20000, 1, 1000},
                 kick.props.cut_a,
                 kick.props.cut_d,
                 kick.props.cut_s,
                 kick.props.cut_r)) {
            kick.adsr_cut.set(kick.props.cut_a, kick.props.cut_d, kick.props.cut_s, kick.props.cut_r);
        }
        ImGui::PushItemWidth(65);
        ImGui::DragFloat("mod range##cut", &kick.props.cut_range, kick.props.cut_range * 0.1f, 0, 8, "%.2f");
        ImGui::DragFloat(
                "cut off", &kick.props.cut_fundamental, kick.props.cut_fundamental * 0.1f, 10, 10000, "%.2f");
        ImGui::DragFloat("reso##cut", &kick.props.filt_q, kick.props.filt_q * 0.1f, 0, 10, "%.2f");

        ImGui::PopItemWidth();
        ImGui::TableNextColumn();
        ImGui::EndTable();
    }

    ImGui::PopID();
}
