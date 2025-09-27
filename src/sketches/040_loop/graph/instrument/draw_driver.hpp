#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "driver.hpp"

inline void draw_driver(const char* id, driver& drv) {
    ImGui::PushID(id);
    ImGui::BeginGroup();

    if (ImGui::Button("driver preset")) {
        ImGui::SetClipboardText(drv.props.to_str().c_str());
    }

    ImGuiKnobs::Knob("drive_input", &drv.props.drive_input, 0, 8);
    ImGui::SameLine();
    ImGuiKnobs::Knob("drive_clip", &drv.props.drive_clip, 0, 8);
    ImGui::SameLine();
    ImGuiKnobs::Knob("clip_thresh", &drv.props.clip_thresh, 0, 2);
    ImGui::SameLine();
    ImGuiKnobs::Knob("trim", &drv.props.trim, 0, 8);

    ImGui::EndGroup();
    ImGui::PopID();
}