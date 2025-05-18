// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include "sequence/event.hpp"
#include "sequence/notes.h"

namespace pattern {
std::vector<pattern_t<event_midi>> chord{
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
                 {{.start_time = 1.5, .duration = 0.5, .note = note::E3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::Gs3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::B3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::Cs3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::Fs3 + 12}}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
                 {{.start_time = 3, .duration = 0.5, .note = note::Fs3},
                  {.start_time = 3, .duration = 0.5, .note = note::B3},
                  {.start_time = 3, .duration = 0.5, .note = note::Cs3},
                  {.start_time = 3, .duration = 0.5, .note = note::E3},
                  {.start_time = 3, .duration = 0.5, .note = note::Gs3 + 12}}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
                 {{.start_time = 1.5, .duration = 0.5, .note = note::E3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::Gs3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::B3},
                  //   {.start_time = 1.5, .duration = 0.5, .note = note::A3},
                  {.start_time = 1.5, .duration = 0.5, .note = note::Cs4},
                  {.start_time = 1.5, .duration = 0.5, .note = note::Fs3 + 12},

                  {.start_time = 3, .duration = 0.5, .note = note::Fs3},
                  {.start_time = 3, .duration = 0.5, .note = note::B3},
                  {.start_time = 3, .duration = 0.5, .note = note::Cs3},
                  //   {.start_time = 3, .duration = 0.5, .note = note::Ds4},
                  {.start_time = 3, .duration = 0.5, .note = note::E3},
                  {.start_time = 3, .duration = 0.5, .note = note::Gs3 + 12}}},
};
}  // namespace pattern
