// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <format>
#include <stdexcept>

#include "env_bp_040.hpp"

env_bp_040::env_bp_040(env_bp_040_props new_props) {
    props = new_props;
    if (props.pattern.empty()) {
        throw std::runtime_error("env_bp was not provided a pattern");
    }
    if (props.duration_abs <= 0) {
        throw std::runtime_error(
            std::format("env_bp was not provided a valid duration_abs: {}", props.duration_abs));
    }
    if (props.duration_rel <= 0) {
        throw std::runtime_error(
            std::format("env_bp was not provided a valid duration_rel: {}", props.duration_rel));
    }
    curr_point       = props.pattern.end();
    current_time_abs = 0;
    is_playing       = false;
}

void env_bp_040::key_on() {
    key_on_from(0);
}

void env_bp_040::key_on_from(double from_time_relative) {
    is_playing         = true;
    from_time_relative = std::fmod(from_time_relative, props.duration_rel);
    if (from_time_relative < 0)
        from_time_relative += props.duration_rel;

    current_time_abs = (from_time_relative / props.duration_rel) * props.duration_abs;
    curr_point       = props.pattern.begin();

    // Find the latest event that happens at or before from_time_relative
    while (curr_point != props.pattern.end() && curr_point->start <= from_time_relative) {
        curr_point += 1;
    }
    curr_point -= 1;
}

void env_bp_040::key_off() {
    is_playing = false;
}

float env_bp_040::tick() {
    auto next_point = curr_point + 1;
    if (next_point == props.pattern.end()) {
        increment_time();
        return curr_point->value;
    }

    double next_point_start_abs = (next_point->start / props.duration_rel) * props.duration_abs;
    while (int(current_time_abs) == int(next_point_start_abs)) {
        // handles jumps, e.g. simultaneous events
        curr_point += 1;
        next_point = curr_point + 1;

        if (next_point == props.pattern.end()) {
            increment_time();
            return curr_point->value;
        }

        next_point_start_abs = (next_point->start / props.duration_rel) * props.duration_abs;
    }

    // curr_segment_duration_abs is never zero because
    // simultaneous events were processed in the while loop above
    double curr_point_start_abs      = (curr_point->start / props.duration_rel) * props.duration_abs;
    double curr_segment_duration_abs = next_point_start_abs - curr_point_start_abs;

    double curr_segment_progress_abs = current_time_abs - curr_point_start_abs;
    double lerp_amount               = curr_segment_progress_abs / curr_segment_duration_abs;

    increment_time();

    return std::lerp(curr_point->value, next_point->value, float(lerp_amount));
}

void env_bp_040::increment_time() {
    if (is_playing) {
        current_time_abs += 1;
        if (current_time_abs >= props.duration_abs) {
            current_time_abs -= props.duration_abs;
            curr_point = props.pattern.begin();
        }
    }
}