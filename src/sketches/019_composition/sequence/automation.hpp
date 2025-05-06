#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "lib/env_bp/env_bp.hpp"

// clang-format off
struct automation_patterns {

        //     {.start_time = 0,   .pattern_index = 3},
        // {.start_time = 32,  .pattern_index = 0},
        // {.start_time = 42,  .pattern_index = 2},
        // {.start_time = 97,  .pattern_index = 3},
        // {.start_time = 129, .pattern_index = 2},
        // {.start_time = 137, .pattern_index = 1},
        // {.start_time = 161, .pattern_index = 2},
        // {.start_time = 161, .pattern_index = 0},
    std::vector<bp_event_000> bp_env_lead_a {
        {.start = 0,   .value = 1},
        {.start = 24,  .value = 1},
        {.start = 30,  .value = 0},
        {.start = 41,  .value = 0},
        {.start = 49,  .value = 1},
        {.start = 93,  .value = 1},
        {.start = 125, .value = 1},
        {.start = 127, .value = 0},
        {.start = 129, .value = 0},
        {.start = 133, .value = 1},
    };

    std::vector<bp_event_000> bp_env_lead_b {
        {.start = 0,   .value = 0},
        {.start = 89,  .value = 0},
        {.start = 97,  .value = 1},
        {.start = 125, .value = 1},
        {.start = 127, .value = 0},
        {.start = 136, .value = 1},
        {.start = 137, .value = 0},
        {.start = 137, .value = 1},
        {.start = 159, .value = 1},
        {.start = 161, .value = 0},
        {.start = 161, .value = 1},
    };

            // {.start_time = 0,   .pattern_index = 0},
            // {.start_time = 32,  .pattern_index = 1},
            // {.start_time = 133, .pattern_index = 0},
            // {.start_time = 137, .pattern_index = 1},
            // {.start_time = 177, .pattern_index = 0},
            // {.start_time = 185, .pattern_index = 1},
    std::vector<bp_event_000> bp_env_kick_hp {
        /*
        mod range in octaves
        - 0   = 10 hz
        - 3.5 = 113 hz
        - 4   = 160 hz
        - 6   = 640 hz
        */
        {.start = 0,    .value = 6},
        {.start = 16,   .value = 4},
        {.start = 30,   .value = 3.5},
        {.start = 32,   .value = 0},

        {.start = 121,  .value = 0},
        {.start = 121,  .value = 1.5},
        {.start = 133,  .value = 3.5},
        {.start = 135,  .value = 1.5},
        {.start = 137,  .value = 0},

        {.start = 177,  .value = 0},
        {.start = 177,  .value = 3.5},
        {.start = 183,  .value = 1.5},
        {.start = 184,  .value = 0},
    };

    std::vector<bp_event_000> bp_env_kick_verb_send {
        {.start = 0,   .value = 0.0},
        // {.start = 0.0,   .value = 1},
        // {.start = 0.5,   .value = 1},
        // {.start = 0.5,   .value = 0},

        // {.start = 4.0,   .value = 1},
        // {.start = 4.5,   .value = 1},
        // {.start = 4.5,   .value = 0},

        // {.start = 8.0,   .value = 1},
        // {.start = 8.5,   .value = 1},
        // {.start = 8.5,   .value = 0},

        // {.start = 12.0,   .value = 1},
        // {.start = 12.5,   .value = 1},
        // {.start = 12.5,   .value = 0},

        // {.start = 16.0,   .value = 1},
        // {.start = 16.5,   .value = 1},
        // {.start = 16.5,   .value = 0},

        // {.start = 20.0,   .value = 1},
        // {.start = 20.5,   .value = 1},
        // {.start = 20.5,   .value = 0},


        {.start = 24,  .value = 0.6},
        {.start = 30,  .value = 1.0},
    };

};
// clang-format on
