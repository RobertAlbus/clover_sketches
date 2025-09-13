// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include "sequence/event.hpp"
#include "sequence/notes.h"

std::vector<pattern_t<event_midi>> patterns::create_pattern_chord() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 3.,
         .pattern =
             {
                 {.start_time = 1.5, .duration = 0.25, .note = note::F4},
                 {.start_time = 1.5, .duration = 0.25, .note = note::Gs4},
                 {.start_time = 1.75, .duration = 0.25, .note = note::F4},
                 {.start_time = 1.75, .duration = 0.25, .note = note::Gs4},
                 {.start_time = 2.25, .duration = 0.25, .note = note::F4},
                 {.start_time = 2.25, .duration = 0.25, .note = note::Gs4},
             }},
        {.duration_bar = 4,
         .duration_rel = 16.,
         .pattern =
             {
                 //
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::C4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::Eb4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::G4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::Bb4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::F4},

                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::C4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::Eb4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::G4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::Bb4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::F4},

                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::C4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::Eb4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::G4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::Bb4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::F4},

                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::F4},

                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::F4},
             }},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //

                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::F4},

                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::F4},
             }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
             {
                 //
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::C4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::Eb4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::G4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::Bb4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::F4},
             }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
             {
                 //
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::C4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::Eb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::G4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::Bb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::F4},
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

                 //
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::C4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::Eb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::G4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::Bb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::F4},
             }},
    };
}

std::vector<pattern_t<event_midi>> patterns::create_pattern_bass() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 0.75,
         .duration_rel = 3.,
         .pattern =
             {
                 // clang-format off
                 {.start_time = 1.5,  .duration = 0.25, .note = note::G2},
                 {.start_time = 1.75, .duration = 0.25, .note = note::G2},
                 {.start_time = 2.25, .duration = 0.25, .note = note::G2},
                //  {.start_time = 3.5,  .duration = 0.25, .note = note::G2},

                //  {.start_time = 4.5,  .duration = 0.25, .note = note::G2},
                //  {.start_time = 5.75, .duration = 0.25, .note = note::G2},
                //  {.start_time = 6.25, .duration = 0.25, .note = note::G2},

                 // clang-format on
             }},
    };
}
