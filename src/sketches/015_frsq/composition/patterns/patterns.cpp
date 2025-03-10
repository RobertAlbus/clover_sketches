// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <unordered_map>

#include "composition/patterns/notes.h"
#include "patterns.hpp"

namespace pattern {

// patterns.hpp:
// const int X = 2;
// const int x = 1;
// const int _ = -1;

int beat = 4;  // whole:       quarter
int qvr  = 2;  // quaver:      eigth
int sqvr = 1;  // semi-quaver: sixteenth
std::unordered_map<int, int> kick_map{
        {0 * beat, X},
        {1 * beat, X},
        {2 * beat, X},
        {3 * beat, X},

        {(0 * beat) + (3 * sqvr), _},
        {(1 * beat) + (3 * sqvr), _},
        {(2 * beat) + (3 * sqvr), _},
        {(3 * beat) + (3 * sqvr), _},
};

std::unordered_map<int, int> hh_map{
        {(0 * beat) + qvr, X},
        {(1 * beat) + qvr, X},
        {(2 * beat) + qvr, X},
        {(3 * beat) + qvr, X},
        //
        {0 * beat, _},
        {1 * beat, _},
        {2 * beat, _},
        {3 * beat, _},
};

constexpr std::array<float, 4> chord     = {note::Gb4, note::Ab4, note::B4, note::Eb5};
constexpr std::array<float, 4> chord_off = {_, _, _, _};
std::unordered_map<int, std::array<float, 4>> chord_map{
        {5 * sqvr, chord},
        {6 * sqvr, chord_off},
        {10 * sqvr, chord},
        {14 * sqvr, chord_off},
};

// clang-format off
std::vector<midi_event> drone_pattern {
        {.start_time = 0,  .duration = 16., .note = note::Eb6 - 36},
        {.start_time = 0,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 1,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 2,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 3,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 4,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 5,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 6,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 7,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 8,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 9,  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 10, .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 11, .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 12, .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 13, .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 14, .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 15, .duration = 0.5, .note = note::Eb7 - 36},
};
// clang-format on

}  // namespace pattern
