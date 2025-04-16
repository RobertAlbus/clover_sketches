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
            {.start_time = 1.5, .duration = 0.5, .note = note::E3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Gs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::B3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Cs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Fs3 + 12},
        },
        {
            {.start_time = 3, .duration = 0.5, .note = note::Fs3},
            {.start_time = 3, .duration = 0.5, .note = note::B3},
            {.start_time = 3, .duration = 0.5, .note = note::Cs3},
            {.start_time = 3, .duration = 0.5, .note = note::E3},
            {.start_time = 3, .duration = 0.5, .note = note::Gs3 + 12},
        },
        {
            {.start_time = 1.5, .duration = 0.5, .note = note::E3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Gs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::B3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Cs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Fs3 + 12},

            {.start_time = 3, .duration = 0.5, .note = note::Fs3},
            {.start_time = 3, .duration = 0.5, .note = note::B3},
            {.start_time = 3, .duration = 0.5, .note = note::Cs3},
            {.start_time = 3, .duration = 0.5, .note = note::E3},
            {.start_time = 3, .duration = 0.5, .note = note::Gs3 + 12},
        },

    };

    // clang-format on
};