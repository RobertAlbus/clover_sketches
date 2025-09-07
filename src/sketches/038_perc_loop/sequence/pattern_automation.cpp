// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include <vector>

std::vector<pattern_t<event>> patterns::create_pattern_sc_pump() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern      = {
             //
             {.start_time = 0, .duration = 0.5},
             {.start_time = 1, .duration = 0.5},
             {.start_time = 2, .duration = 0.5},
             {.start_time = 3, .duration = 0.5},
         }}};
}
