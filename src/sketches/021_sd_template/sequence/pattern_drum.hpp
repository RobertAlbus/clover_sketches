#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "sequence/event.hpp"

// clang-format off
struct pattern_drum {
    std::vector<std::vector<event>> patterns_kick{
        {},
        {
            {.start_time = 0, .duration = 0.5},
            {.start_time = 1, .duration = 0.5},
            {.start_time = 2, .duration = 0.5},
            {.start_time = 3, .duration = 0.5},
        },
    };

    // clang-format on
};