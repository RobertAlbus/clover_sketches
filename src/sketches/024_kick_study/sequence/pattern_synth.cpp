// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include "sequence/event.hpp"
#include "sequence/notes.h"

namespace pattern {

std::vector<pattern_t<event_midi>> chord{
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
                 {
                         //
                         {.start_time = 1.5, .duration = 0.75, .note = -17 + note::C4},
                         {.start_time = 1.5, .duration = 0.75, .note = -17 + note::Eb4},
                         {.start_time = 1.5, .duration = 0.75, .note = -17 + note::G4},
                         {.start_time = 1.5, .duration = 0.75, .note = -17 + note::Bb4},
                         {.start_time = 1.5, .duration = 0.75, .note = -17 + note::F4},
                 }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
                 {
                         // (F–A–C–E♭–G♯)
                         {.start_time = 3, .duration = 0.75, .note = note::F4},
                         {.start_time = 3, .duration = 0.75, .note = note::A4},
                         {.start_time = 3, .duration = 0.75, .note = note::C4},
                         {.start_time = 3, .duration = 0.75, .note = note::Eb4},
                         {.start_time = 3, .duration = 0.75, .note = note::Gs4},
                 }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
                 {
                         //
                         {.start_time = 1.5, .duration = 0.75, .note = note::C4},
                         {.start_time = 1.5, .duration = 0.75, .note = note::Eb4},
                         {.start_time = 1.5, .duration = 0.75, .note = note::G4},
                         {.start_time = 1.5, .duration = 0.75, .note = note::Bb4},
                         {.start_time = 1.5, .duration = 0.75, .note = note::F4},

                         {.start_time = 6.5, .duration = 0.75, .note = note::F4},
                         {.start_time = 6.5, .duration = 0.75, .note = note::A4},
                         {.start_time = 6.5, .duration = 0.75, .note = note::C4},
                         {.start_time = 6.5, .duration = 0.75, .note = note::Eb4},
                         {.start_time = 6.5, .duration = 0.75, .note = note::Gs4},
                 }},
};
}  // namespace pattern
