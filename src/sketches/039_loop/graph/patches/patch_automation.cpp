// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patches.hpp"

patch_automation_t::patch_automation_t(float fs, float bpm) {
    // --------------------------------
    // SIDECHAIN

    const float samples_per_beat = (fs * 60) / bpm;

    sc_pump_props = {
        .duration_abs = samples_per_beat,
        .duration_rel = 4,
        .envelopes    = {
            {{0, 1}, {0.1, 0}, {0.5, 0}, {1, 1}},  // tight ducking
            {
                {0, 1},
                {0.1, 0},
                {1, 0},
                {3, 0.2},
                {3.8, 1},
            },  // pumping
        }};
}
