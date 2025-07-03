// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
using namespace ImGui;

#include "drag_slider_000.hpp"

bool drag_slider_v_000(
    const char* id,
    float& value,
    float min,
    float max,
    const char* fmt,
    ImGuiSliderFlags slider_flags,
    const ImVec2& dimensions) {
    ImGui::PushID(id);
    slider_flags |= ImGuiSliderFlags_NoRoundToFormat;

    bool was_modified = VSliderFloat("##slider", dimensions, &value, min, max, fmt, slider_flags);

    ImGui::PushItemWidth(dimensions.x);
    was_modified = was_modified || ImGui::DragFloat("##spinner", &value, max * 0.2f, min, max, fmt);
    ImGui::PopItemWidth();

    ImGui::PopID();

    return was_modified;
}
