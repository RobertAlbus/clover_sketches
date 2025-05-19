// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
using namespace ImGui;

bool draw_drag_slider(float& control, float control_min, float control_max, float available_width) {
    PushID(&control);

    float control_range = (control_max - control_min);

    auto drag_fmt = control < 1000.f ? "%.2f" : "%.f";
    PushItemWidth(available_width * 0.1f);
    bool changed_drag = DragFloat(
            "##drag",
            &control,
            control * 0.01f,
            control_min,
            control_max,
            drag_fmt,
            ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    PopItemWidth();
    SameLine();
    PushItemWidth(available_width * 0.9f);
    float slider_control = (control - control_min) / control_range;

    bool changed_slider = SliderFloat(
            "##slider",
            &slider_control,
            0,
            1,
            "",
            ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
    PopItemWidth();
    PopID();
    control = (slider_control * control_range) + control_min;
    if (changed_slider || changed_drag) {
        control = (slider_control * control_range) + control_min;
    }
    return changed_slider || changed_drag;
};
