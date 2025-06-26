#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <span>

struct bp_event_000 {
    double start;
    float value;
};

// this could be re-written better
// now that I have designed frsq_024
struct env_bp_000 {
    double duration_abs;
    double duration_rel;
    double current_time_abs;

    using pattern_t = std::span<bp_event_000>;

    env_bp_000();
    env_bp_000(pattern_t new_pattern);

    pattern_t pattern;
    pattern_t::iterator curr_point;

    void set_pattern(pattern_t new_pattern);
    void key_on();

    float tick();
};
