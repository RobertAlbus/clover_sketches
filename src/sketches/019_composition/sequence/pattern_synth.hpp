#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "sequence/event.hpp"
#include "sequence/notes.h"

// clang-format off
struct pattern_synth {
    std::vector<std::vector<event_midi>> patterns_chord {
        {},
        {
            {.start_time = 1.5, .duration = 0.5, .note = note::Ds3},
            {.start_time = 1.5, .duration = 0.5, .note = note::E3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Fs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::B4},
            {.start_time = 1.5, .duration = 0.5, .note = note::Ds4},
            {.start_time = 1.5, .duration = 0.5, .note = note::E4},
        },
        {
            {.start_time = 3, .duration = 0.5, .note = note::Fs3},
            {.start_time = 3, .duration = 0.5, .note = note::A4},
            {.start_time = 3, .duration = 0.5, .note = note::B4},
            {.start_time = 3, .duration = 0.5, .note = note::Ds4},
            {.start_time = 3, .duration = 0.5, .note = note::E4},
            {.start_time = 3, .duration = 0.5, .note = note::Fs4},
        },
        {
            {.start_time = 1.5, .duration = 0.5, .note = note::Ds3},
            {.start_time = 1.5, .duration = 0.5, .note = note::E3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Fs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::B4},
            {.start_time = 1.5, .duration = 0.5, .note = note::Ds4},
            {.start_time = 1.5, .duration = 0.5, .note = note::E4},

            {.start_time = 3, .duration = 0.5, .note = note::Fs3},
            {.start_time = 3, .duration = 0.5, .note = note::A4},
            {.start_time = 3, .duration = 0.5, .note = note::B4},
            {.start_time = 3, .duration = 0.5, .note = note::Ds4},
            {.start_time = 3, .duration = 0.5, .note = note::E4},
            {.start_time = 3, .duration = 0.5, .note = note::Fs4},
        },
    };

    std::vector<std::vector<event_midi>> patterns_lead {
        {},
        {
            {.start_time = 0,  .duration = 0.2, .note = note::E3},
            {.start_time = 1,  .duration = 0.2, .note = note::E3},
            {.start_time = 2,  .duration = 0.2, .note = note::E3},
            {.start_time = 3,  .duration = 0.2, .note = note::E4 + 0.55},
            {.start_time = 4,  .duration = 0.2, .note = note::E3},
            {.start_time = 5,  .duration = 0.2, .note = note::E3},
            {.start_time = 6,  .duration = 0.2, .note = note::E3},
            {.start_time = 7,  .duration = 0.2, .note = note::E3},
            {.start_time = 8,  .duration = 0.2, .note = note::E3},
            {.start_time = 9,  .duration = 0.2, .note = note::E3},
            {.start_time = 10, .duration = 0.2, .note = note::E3},
            {.start_time = 11, .duration = 0.2, .note = note::E3},
            {.start_time = 12, .duration = 0.2, .note = note::E4 + 0.55},
            {.start_time = 13, .duration = 0.2, .note = note::E3},
            {.start_time = 14, .duration = 0.2, .note = note::E3},
            {.start_time = 15, .duration = 0.2, .note = note::E3},
        },
    };
    // clang-format on
};