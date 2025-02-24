// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "gui/adsr.hpp"

#include "kick_drum_gui.hpp"

void kick_drum_gui(const char* id, kick_props& kick) {
    ImGui::PushID(id);

    if (ImGui::Button("get kick patch")) {
        ImGui::SetClipboardText(kick.to_str().c_str());
    }

    ImGui::BeginTable("##table", 4);

    ImGui::TableSetupColumn("amp", ImGuiTableColumnFlags_WidthFixed, 150);
    ImGui::TableSetupColumn("pitch", ImGuiTableColumnFlags_WidthFixed, 150);
    ImGui::TableSetupColumn("saturation", ImGuiTableColumnFlags_WidthFixed, 150);
    ImGui::TableSetupColumn("filter", ImGuiTableColumnFlags_WidthFixed, 150);
    ImGui::TableHeadersRow();

    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    adsr("##amp_asr", {1000, 40000, 1, 1000}, kick.amp_a, kick.amp_d, kick.amp_s, kick.amp_r);
    ImGui::TableNextColumn();
    adsr("##pitch_asr", {1000, 10000, 1, 1000}, kick.pitch_a, kick.pitch_d, kick.pitch_s, kick.pitch_r);
    ImGui::PushItemWidth(65);
    if (ImGui::DragFloat(
                "mod range##pitch", &kick.pitch_peak.gui, kick.pitch_peak.gui * 0.1f, 10, 10000, "%.f")) {
        kick.pitch_peak.set(kick.pitch_peak.gui);
    }
    if (ImGui::DragFloat(
                "fundamental##pitch",
                &kick.pitch_fundamental.gui,
                kick.pitch_fundamental.gui * 0.1f,
                10,
                10000,
                "%.f")) {
        kick.pitch_fundamental.set(kick.pitch_fundamental.gui);
    }

    ImGui::PopItemWidth();
    ImGui::TableNextColumn();

    float drive_gain_min = 0;
    float drive_gain_max = 10;
    float trim_gain_min  = 0;
    float trim_gain_max  = 1;

    bool drive_changed = ImGui::VSliderFloat(
            "##drive_slider", ImVec2(30, 100), &kick.drive.gui, drive_gain_min, drive_gain_max);
    ImGui::SameLine();
    bool trim_changed = ImGui::VSliderFloat(
            "##trim_slider", ImVec2(30, 100), &kick.trim.gui, trim_gain_min, trim_gain_max);

    ImGui::PushItemWidth(30);
    drive_changed =
            drive_changed ||
            ImGui::DragFloat(
                    "##drive_drag", &kick.drive.gui, 0.2f * kick.trim.gui, drive_gain_min, drive_gain_max);
    ImGui::SameLine();
    trim_changed = trim_changed ||
                   ImGui::DragFloat(
                           "##trim_drag", &kick.trim.gui, 0.2f * kick.trim.gui, trim_gain_min, trim_gain_max);

    if (drive_changed) {
        kick.drive.set(kick.drive.gui);
    }
    if (trim_changed) {
        kick.trim.set(kick.trim.gui);
    }
    ImGui::PopItemWidth();

    ImGui::TableNextColumn();
    adsr("##cut_asr", {1000, 20000, 1, 1000}, kick.cut_a, kick.cut_d, kick.cut_s, kick.cut_r);
    ImGui::PushItemWidth(65);
    if (ImGui::DragFloat("mod range##cut", &kick.cut_peak.gui, kick.cut_peak.gui * 0.1f, 10, 10000, "%.f")) {
        kick.cut_peak.set(kick.cut_peak.gui);
    }
    if (ImGui::DragFloat(
                "cut off", &kick.cut_fundamental.gui, kick.cut_fundamental.gui * 0.1f, 10, 10000, "%.f")) {
        kick.cut_fundamental.set(kick.cut_fundamental.gui);
    }
    if (ImGui::DragFloat("reso##cut", &kick.filt_q.gui, kick.filt_q.gui * 0.1f, 0, 10, "%.2f")) {
        kick.filt_q.set(kick.filt_q.gui);
    }

    ImGui::PopItemWidth();
    ImGui::TableNextColumn();
    ImGui::EndTable();

    ImGui::PopID();
}
