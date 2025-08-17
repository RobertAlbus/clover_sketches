// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "env_bp.hpp"
#include <cmath>
#include <stdexcept>

std::array<bp_event_000, 2> default_env_bp_pattern{{
    {.start = 0, .value = 0},
    {.start = 1, .value = 0},
}};

env_bp_000::env_bp_000() {
    set_pattern(default_env_bp_pattern);
    duration_abs = 1;
    duration_rel = 1;
    curr_point   = pattern.end();
}

void env_bp_000::set_pattern(pattern_t new_pattern) {
    /*
    todo: pattern validation
    - must start at time 0
    - max 2 events in a row with same start time?
    */
    pattern    = new_pattern;
    curr_point = pattern.end();
}

env_bp_000::env_bp_000(pattern_t new_pattern) {
    set_pattern(new_pattern);
}

void env_bp_000::key_on() {
    curr_point       = pattern.begin();
    current_time_abs = 0;
}

float env_bp_000::tick() {
    if (pattern.empty())
        throw std::runtime_error("env_bp was not provided a pattern");

    if (curr_point == pattern.end()) {
        return pattern[0].value;
    }
    auto next_point = curr_point + 1;
    if (next_point == pattern.end()) {
        return curr_point->value;
    }

    // handles breakpoint update or a jump due to simultaneous events
    double next_point_start_abs = (next_point->start / duration_rel) * duration_abs;
    while (int(current_time_abs) == int(next_point_start_abs)) {
        curr_point += 1;
        next_point = curr_point + 1;

        if (next_point == pattern.end()) {
            return curr_point->value;
        }

        next_point_start_abs = (next_point->start / duration_rel) * duration_abs;
    }

    double curr_point_start_abs = (curr_point->start / duration_rel) * duration_abs;

    // curr_segment_duration_abs is never zero because of the while loop
    double curr_segment_duration_abs = next_point_start_abs - curr_point_start_abs;

    double curr_segment_progress_abs = current_time_abs - curr_point_start_abs;
    double lerp_amount               = curr_segment_progress_abs / curr_segment_duration_abs;

    current_time_abs += 1;

    return std::lerp(curr_point->value, next_point->value, float(lerp_amount));
}
