#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "sequence/event.hpp"
#include "sequence/notes.h"

// clang-format off
struct pattern_drum {
    std::vector<std::vector<event>> patterns_kick{
        {},
        {
            {.start_time = 0, .duration = 0.5},
        },
    };

    std::vector<std::vector<event_midi>> patterns_bass{
        {},
        {
            {.start_time = 0.05 + 0.5, .duration = 0.20, .note = note::Fs3},
            {.start_time = 0.05 + 1.0, .duration = 0.20, .note = note::F3},
            {.start_time = 0.05 + 3.5, .duration = 0.20, .note = note::F3},
            {.start_time = 0.05 + 4.5, .duration = 0.20, .note = note::Fs3},
            {.start_time = 0.05 + 5.0, .duration = 0.20, .note = note::F3},
        },
    };

    std::vector<std::vector<event>> patterns_hh1{
        {},
        {
            {.start_time = 0.50, .duration = 0.25},
            {.start_time = 1.50, .duration = 0.20},
            {.start_time = 1.75, .duration = 0.25},
        },
    };

    std::vector<std::vector<event>> patterns_hh2{
        {},
        {
            {.start_time = 0.002, .duration = 0.1},
            {.start_time = 0.252, .duration = 0.1},
            {.start_time = 0.502, .duration = 0.1},
            {.start_time = 1.002, .duration = 0.1},
            {.start_time = 1.502, .duration = 0.1},
        },
    };

    std::vector<std::vector<event>> patterns_hh3{
        {},
        {
            {.start_time = 1.752, .duration = 0.5},
            {.start_time = 3.752, .duration = 0.5},
            {.start_time = 5.752, .duration = 0.5},
        },
    };

    std::vector<std::vector<event>> patterns_ride{
        {},
        {
            {.start_time = 0.0, .duration = 0.5},
            {.start_time = 0.5, .duration = 0.5},
        },
    };
    // clang-format on
};