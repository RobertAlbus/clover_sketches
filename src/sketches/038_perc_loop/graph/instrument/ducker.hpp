#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "lib/env_bp/env_bp.hpp"

constexpr size_t ducker_size = 2;
struct ducker_props {
    float duration_abs;
    float duration_rel;
    std::vector<std::vector<bp_event_000>> envelopes;
};

struct ducker {
    ducker_props props;

    ducker(ducker_props& props);
    void patch(ducker_props new_props);
    void key_on();
    void key_on_kick();

    std::vector<env_bp_000> envs;       // always pulsing
    std::vector<env_bp_000> envs_kick;  // triggered by kick
    std::vector<float> xs;
    std::vector<float> xs_kick;
    void tick();
};
