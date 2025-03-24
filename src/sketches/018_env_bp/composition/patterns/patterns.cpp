// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <array>
#include <unordered_map>

#include "composition/instruments/env_bp.hpp"
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

// clang-format off
std::vector<midi_event> hh2_pattern {
        {.start_time = 0, .duration = 0.9},
        {.start_time = 1, .duration = 0.9},
        {.start_time = 2, .duration = 0.9},
        {.start_time = 3, .duration = 0.9},
};
std::vector<midi_event> hh2_articulation_trigger_pattern {
        {.start_time = 0, .duration = 0.1},
};
std::vector<bp_event> hh2_articulation_env_pattern {
        bp_event{.start = 0.0, .value = 0},
        bp_event{.start = 1.0, .value = 0},
        bp_event{.start = 2.0, .value = 1},
        bp_event{.start = 2.5, .value = 1},
        bp_event{.start = 3.0, .value = 0.5},
        bp_event{.start = 4.0, .value = 0.5},
};

// clang-format off

constexpr std::array<float, 4> chord     = {note::Gb4, note::Ab4, note::B4, note::Eb5};
constexpr std::array<float, 4> chord_off = {_, _, _, _};
std::unordered_map<int, std::array<float, 4>> chord_map{
        {5 * sqvr, chord},
        {6 * sqvr, chord_off},
        {10 * sqvr, chord},
        {14 * sqvr, chord_off},
};

bool midi_start_time_sort(midi_event& a, midi_event& b) {
    return a.start_time < b.start_time;  // ascending order
}

// clang-format off
std::vector<midi_event> beep_pattern_1 {
        {.start_time = 0,              .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 1 * (3.*sqvr),  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 2 * (3.*sqvr),  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 3 * (3.*sqvr),  .duration = 0.5, .note = note::Eb7 - 36},
        {.start_time = 4 * (3.*sqvr),  .duration = 0.5, .note = note::Eb7 - 36},
};

std::vector<midi_event> beep_pattern_2 {
        {.start_time = (0 * (3.*sqvr)),  .duration = 0.5, .note = note::Gb7 - 36},
        {.start_time = (1 * (3.*sqvr)),  .duration = 0.5, .note = note::Gb7 - 36},
        {.start_time = (2 * (3.*sqvr)),  .duration = 0.5, .note = note::E7  - 36},
        {.start_time = (3 * (3.*sqvr)),  .duration = 0.5, .note = note::E7  - 36},
        {.start_time = (4 * (3.*sqvr)),  .duration = 0.5, .note = note::Eb7 - 36},
};

std::vector<midi_event> beep_pattern_3 = []() {
    std::vector<midi_event> pattern{
            {.start_time = (0 * (3. * sqvr)), .duration = 0.5, .note = note::Gb7 - 36},
            {.start_time = (1 * (3. * sqvr)), .duration = 0.5, .note = note::Gb7 - 36},
            {.start_time = (2 * (3. * sqvr)), .duration = 0.5, .note = note::E7 - 36},
            {.start_time = (3 * (3. * sqvr)), .duration = 0.5, .note = note::E7 - 36},
            {.start_time = (4 * (3. * sqvr)), .duration = 0.5, .note = note::Eb7 - 36},

            {.start_time = (0 * (3. * sqvr)) + 1, .duration = 0.5, .note = note::Gb7 - 36},
        //     {.start_time = (1 * (3. * sqvr)) + 1, .duration = 0.5, .note = note::Gb7 - 36},
            {.start_time = (2 * (3. * sqvr)) + 1, .duration = 0.5, .note = note::E7  - 36},
            {.start_time = (3 * (3. * sqvr)) + 1, .duration = 0.5, .note = note::Eb7 - 36},
        //     {.start_time = (4 * (3. * sqvr)) + 1, .duration = 0.5, .note = note::Eb7 - 36},
    };
    std::sort(pattern.begin(), pattern.end(), midi_start_time_sort);

    return pattern;
}();
// clang-format on

std::array<std::vector<midi_event>, 3> beep_patterns{beep_pattern_1, beep_pattern_2, beep_pattern_3};

std::vector<meta_pattern> beep_meta_pattern{
        {.start_time = 0, .pattern_index = 0},
        {.start_time = 3, .pattern_index = 1},
        {.start_time = 4, .pattern_index = 0},
        {.start_time = 7, .pattern_index = 2},
};

}  // namespace pattern
