#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "draw_adsr.hpp"
#include "imgui-knobs.h"
#include "lib/_atom/adsr/adsr.hpp"

bool draw_adsr_038(
    const char* id,
    const adsr_ranges_000& ranges,
    adsr_values& adsr,
    float graph_width         = 200.0f,
    float graph_height        = 100.0f,
    float knob_size           = 0.f,
    ImGuiKnobFlags knob_flags = ImGuiKnobFlags_NoTitle);
