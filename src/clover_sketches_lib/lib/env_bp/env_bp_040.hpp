#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "lib/env_bp/env_bp.hpp"

struct env_bp_040_props {
    std::vector<bp_event_000> pattern;
    double duration_abs;  // in samples
    double duration_rel;  // in bars
};

struct env_bp_040 {
    env_bp_040_props props;
    std::vector<bp_event_000>::iterator curr_point;
    double current_time_abs;
    bool is_playing = false;

    env_bp_040(env_bp_040_props);

    void key_on();
    void key_on_from(double from_time_relative);
    void key_off();

    float tick();
    void increment_time();
};
