#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

bool slider_spinnner_v(
        const char* id,
        float& value,
        float min,
        float max,
        const char* fmt,
        ImGuiSliderFlags slider_flags,
        const ImVec2& dimensions);

bool slider_spinnner_v(
        const char* id,
        float& value,
        float min,
        float max,
        const char* fmt,
        ImGuiSliderFlags slider_flags = 0);

struct adsr_ranges {
    float a_max;
    float d_max;
    float s_max;
    float r_max;
};

bool adsr(const char* id, const adsr_ranges& ranges, float& a, float& d, float& s, float& r);