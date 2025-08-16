#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "lib/_molecule/adsr/draw_adsr.hpp"

bool draw_adsr_knobs_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float& a,
    float& d,
    float& s,
    float& r,
    float knob_size           = 0,
    ImGuiKnobFlags knob_flags = 0);