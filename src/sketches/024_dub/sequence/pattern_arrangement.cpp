// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <vector>

#include "patterns.hpp"

namespace arrangement {
double playback_start = 0;

// clang-format off
std::vector<event_meta_sq> kick{
        {.start_time = 0, .pattern_index = 0},
        {.start_time = 0, .pattern_index = 1},
};

std::vector<event_meta_sq> ride{
        {.start_time = 0,  .pattern_index = 1},
        {.start_time = 24, .pattern_index = 1},
};

std::vector<event_meta_sq> chord{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 0, .pattern_index = 1},
};

// TODO
// - patterns should be a constructable object
// - use ctx to configure the duration of the print statement   
std::vector<event> bar = std::views::iota(0, 32) |
                         std::views::transform([](int i)
                            { return event{.start_time = double(i)}; }) |
                         std::ranges::to<std::vector>();

std::vector<bp_event_000> bp_env_kick_hp{
        /*
        mod range in octaves
        - 0   = 10 hz
        - 3.5 = 113 hz
        - 4   = 160 hz
        - 6   = 640 hz
        */
        {.start = 0, .value = 0},
        {.start = 14, .value = 4}
};

std::vector<bp_event_000> bp_env_kick_verb_send{
        {.start = 0, .value = 1.0},
        {.start = 1, .value = 1.0}
};

// clang-format on
}  // namespace arrangement
