#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

struct adsr_ranges_000 {
    float a_max;
    float d_max;
    float s_max;
    float r_max;
};

bool draw_adsr_000(const char* id, const adsr_ranges_000& ranges, float& a, float& d, float& s, float& r);