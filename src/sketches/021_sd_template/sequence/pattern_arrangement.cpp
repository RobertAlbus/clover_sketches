// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "patterns.hpp"

namespace arrangement {
double playback_start = 0;

std::vector<event_meta_sq> kick{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 16, .pattern_index = 1},
};

std::vector<event_meta_sq> chord{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 12, .pattern_index = 3},
        {.start_time = 16, .pattern_index = 2},
        {.start_time = 28, .pattern_index = 3},
};

std::vector<event> bar = std::views::iota(0, 32) |
                         std::views::transform([](int i) { return event{.start_time = double(i)}; }) |
                         std::ranges::to<std::vector>();
}  // namespace arrangement
