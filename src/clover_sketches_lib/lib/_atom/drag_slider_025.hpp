#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

const ImGuiSliderFlags drag_slider_h_025_default_slider_flags =
    ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_ClampOnInput;

bool drag_slider_h_025(
    //
    float& control,
    float min,
    float max,
    ImGuiSliderFlags slider_flags = drag_slider_h_025_default_slider_flags);
bool drag_slider_h_025(
    //
    float& control,
    float min,
    float max,
    const ImVec2& dimensions,
    ImGuiSliderFlags slider_flags = drag_slider_h_025_default_slider_flags);
