#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "lib/_atom/adsr/adsr.hpp"
#include "lib/_molecule/adsr/draw_adsr.hpp"

bool draw_adsr_knobs_038(
    const char* id,
    const adsr_ranges_000& ranges,
    adsr_values& adsr,
    float knob_size           = 0,
    ImGuiKnobFlags knob_flags = ImGuiKnobFlags_NoTitle);
