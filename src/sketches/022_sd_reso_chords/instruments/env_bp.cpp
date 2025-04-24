// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "env_bp.hpp"
#include <cmath>
#include <stdexcept>

env_bp::env_bp(pattern_t new_pattern) {
    set_pattern(new_pattern);
}

void env_bp::set_pattern(pattern_t new_pattern) {
    /*
    todo: pattern validation
        - must start at time 0
        - max 2 events in a row with same start time?
    */
    pattern = new_pattern;
}

void env_bp::key_on() {
    curr_point       = pattern.begin();
    next_point       = curr_point + 1;
    current_time_abs = 0;
}

float env_bp::tick() {
    if (pattern.empty())
        throw std::runtime_error("env_bp was not provided a pattern");
    if (next_point == pattern.end())
        return curr_point->value;

    // handles breakpoint update or a jump due to simultaneous events
    double next_point_start_abs = (next_point->start / duration_rel) * duration_abs;
    while (int(current_time_abs) == int(next_point_start_abs)) {
        curr_point += 1;
        next_point += 1;

        if (next_point == pattern.end())
            return curr_point->value;

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
