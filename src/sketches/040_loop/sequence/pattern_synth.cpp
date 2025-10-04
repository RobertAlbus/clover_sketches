// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include "sequence/event.hpp"
#include "sequence/notes.h"

std::vector<pattern_t<event_midi>> patterns::create_pattern_chord_1() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 3.,
         .pattern =
             {
                 {.start_time = 1.50, .duration = 0.25, .note = -24 + note::F4},
                 {.start_time = 1.50, .duration = 0.25, .note = -24 + note::Gs4},
                 {.start_time = 1.75, .duration = 0.25, .note = -24 + note::F4},
                 {.start_time = 1.75, .duration = 0.25, .note = -24 + note::Gs4},
                 {.start_time = 2.25, .duration = 0.25, .note = -24 + note::F4},
                 {.start_time = 2.25, .duration = 0.25, .note = -24 + note::Gs4},
             }},
    };
}

std::vector<pattern_t<event_midi>> patterns::create_pattern_chord_2() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 2,
         .duration_rel = 6.,
         .pattern =
             {
                 {.start_time = 3., .duration = 1, .note = note::F5},
                 {.start_time = 3., .duration = 1, .note = note::Gs5},
                 {.start_time = 3., .duration = 1, .note = note::C5},
             }},
    };
}

std::vector<pattern_t<event_midi>> patterns::create_pattern_bass() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1.0,
         .duration_rel = 4.,
         .pattern =
             {
                 // clang-format off




                {.start_time = 0.25, .duration = 0.75, .note = note::F3},
                {.start_time = 1.00, .duration = 0.50, .note = note::Gs2},
                {.start_time = 3.50, .duration = 0.50, .note = note::C3},

                 // clang-format on
             }},
    };
}
