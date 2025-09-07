// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/_molecule/adsr/draw_adsr_038.hpp"
#include "lib/kick_drum/kick_drum_038.hpp"

void draw_kick_drum_038(const char* id, kick_drum_038& kick) {
    ImGui::PushID(id);

    if (ImGui::Button("get kick patch")) {
        ImGui::SetClipboardText(kick.props.to_str().c_str());
    }

    ImGui::BeginGroup();
    if (draw_adsr_038("##amp_asr", {1000, 40000, 1, 1000}, kick.props.amp_adsr)) {
        kick.adsr_amp.set(
            kick.props.amp_adsr.a, kick.props.amp_adsr.d, kick.props.amp_adsr.s, kick.props.amp_adsr.r);
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::BeginGroup();
    if (draw_adsr_038("##pitch_asr", {1000, 10000, 1, 1000}, kick.props.pitch_adsr)) {
        kick.adsr_pitch.set(
            kick.props.pitch_adsr.a,
            kick.props.pitch_adsr.d,
            kick.props.pitch_adsr.s,
            kick.props.pitch_adsr.r);
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
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Dummy({20, 0});
    ImGui::SameLine();

    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Dummy({120, 160});
    ImGui::PushItemWidth(65);
    ImGui::DragFloat("drive", &kick.props.drive, kick.props.drive * 0.1f, 0, 10, "%.2f");
    ImGui::DragFloat("trim", &kick.props.trim, kick.props.trim * 0.1f, 0, 10, "%.2f");
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Dummy({20, 0});
    ImGui::SameLine();

    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::BeginGroup();
    if (draw_adsr_038("##cut_asr", {1000, 20000, 1, 1000}, kick.props.cut_adsr)) {
        kick.adsr_cut.set(
            kick.props.cut_adsr.a, kick.props.cut_adsr.d, kick.props.cut_adsr.s, kick.props.cut_adsr.r);
    }
    ImGui::PushItemWidth(65);
    ImGui::DragFloat("mod range##cut", &kick.props.cut_range, kick.props.cut_range * 0.1f, 0, 8, "%.2f");
    ImGui::DragFloat(
        "cut off", &kick.props.cut_fundamental, kick.props.cut_fundamental * 0.1f, 10, 10000, "%.2f");
    ImGui::DragFloat("reso##cut", &kick.props.filt_q, kick.props.filt_q * 0.1f, 0, 10, "%.2f");

    ImGui::EndGroup();

    ImGui::PopID();
}
