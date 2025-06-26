#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>

#include "imgui.h"

#include "lib/env_bp/env_bp.hpp"

struct meter {
    meter();
    float peak      = 0;
    float peak_held = 0;
    float rms       = 0;

    float squared_ema = 0;
    float alpha       = 1e-4;

    env_bp_000 peak_value;
    env_bp_000 peak_hold;
    float peak_value_scale = 0;
    float peak_hold_scale  = 0;
    std::array<bp_event_000, 3> peak_hold_env{
            {{.start = 0, .value = 1}, {.start = 1, .value = 1}, {.start = 1.5, .value = 0}}};

    void tick(float x);
};

void draw_meter(ImVec2 dimensions, float peak, float peak_hold, float rms);
