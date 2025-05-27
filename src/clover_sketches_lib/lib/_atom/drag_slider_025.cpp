// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include <cmath>
using namespace ImGui;

#include "drag_slider_025.hpp"

bool drag_slider_h_025(
        //
        float& control,
        float min,
        float max,
        ImGuiSliderFlags slider_flags) {
    const float available_width = ImGui::GetContentRegionAvail().x;
    const ImVec2 dimensions{available_width, 30};

    return drag_slider_h_025(control, min, max, dimensions, slider_flags);
}

bool drag_slider_h_025(
        //
        float& control,
        float min,
        float max,
        const ImVec2& dimensions,
        ImGuiSliderFlags slider_flags) {
    PushID(&control);

    float available_width = ImGui::GetContentRegionAvail().x;

    float range   = (max - min);
    auto drag_fmt = control < 1000.f ? "%.1f" : control < 100. ? "%.2f" : "%.f";

    const float min_speed   = 10;
    const float curve_exp   = 1.1;
    const float abs_control = std::abs(control);
    float drag_speed        = (std::pow(abs_control, curve_exp) / abs_control) + min_speed;
    if (std::isnan(drag_speed))
        drag_speed = min_speed;
    PushItemWidth(dimensions.x * 0.1f);
    bool changed_drag = DragFloat("##drag", &control, drag_speed, min, max, drag_fmt, slider_flags);
    PopItemWidth();

    SameLine();

    PushItemWidth((dimensions.x * 0.9f) - (ImGui::GetStyle().WindowPadding.x));
    float slider_control = (control - min) / range;
    bool changed_slider  = SliderFloat("##slider", &slider_control, 0, 1, "", slider_flags);
    PopItemWidth();

    control = (slider_control * range) + min;
    if (changed_slider) {
        control = (slider_control * range) + min;
    }

    PopID();

    return changed_slider || changed_drag;
};
