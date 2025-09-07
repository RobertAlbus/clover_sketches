#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/_atom/adsr/adsr.hpp"
#include "lib/_molecule/adsr/draw_adsr.hpp"

void draw_adsr_graph_038(
    const char* id,
    const adsr_ranges_000& ranges,
    const adsr_values& adsr,
    float width  = 200.0f,
    float height = 100.0f);
