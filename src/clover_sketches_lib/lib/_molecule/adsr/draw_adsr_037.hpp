#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "draw_adsr.hpp"
#include "imgui-knobs.h"

bool draw_adsr_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float& a,
    float& d,
    float& s,
    float& r,
    float graphj_width        = 200.0f,
    float graphj_height       = 100.0f,
    float knob_size           = 0.f,
    ImGuiKnobFlags knob_flags = 0);