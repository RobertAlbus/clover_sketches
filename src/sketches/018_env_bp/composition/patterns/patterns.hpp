#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/env_bp.hpp"
#include <unordered_map>
#include <vector>

namespace pattern {

const int X = 2;
const int x = 1;
const int _ = -1;

extern std::unordered_map<int, int> kick_map;
extern std::unordered_map<int, int> hh_map;
extern std::unordered_map<int, std::array<float, 4>> chord_map;

struct midi_event {
    double start_time;
    double duration;
    float note;
};

struct meta_pattern {
    double start_time;
    size_t pattern_index;

    // required for frsq_data_base but not semantically
    // relevant for meta patterns.
    double duration = 1;
};

extern std::array<std::vector<midi_event>, 3> beep_patterns;
extern std::vector<meta_pattern> beep_meta_pattern;
extern std::vector<midi_event> hh2_pattern;
extern std::vector<midi_event> hh2_articulation_trigger_pattern;
extern std::vector<bp_event> hh2_articulation_env_pattern;

}  // namespace pattern
