#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <vector>

#include "sequence/event.hpp"

// clang-format off
struct pattern_meta {
    // WARNING: this won't work for now
    // - env_bp in composition arn't aware of global time.
    // - meta sequencers only initiate patterns at relative time 0.
    double PLAYBACK_START = 0; 
    
    std::vector<event_meta_sq> patterns_kick{
            {.start_time = 0,   .pattern_index = 1},
            {.start_time = 16,  .pattern_index = 1},
    };

    std::vector<event_meta_sq> patterns_chord {
        {.start_time = 0,  .pattern_index = 1},
        {.start_time = 4,  .pattern_index = 3},
        {.start_time = 8,  .pattern_index = 2},
        {.start_time = 12, .pattern_index = 3},
        {.start_time = 16, .pattern_index = 1},
        {.start_time = 20, .pattern_index = 3},
        {.start_time = 24, .pattern_index = 2},
        {.start_time = 28, .pattern_index = 3},
        {.start_time = 32, .pattern_index = 1},
    };

    std::vector<event> timing_range = std::views::iota(0, 32)
                    | std::views::transform([](int i) { return event{.start_time = double(i)}; })
                    | std::ranges::to<std::vector>();

};
// clang-format on
