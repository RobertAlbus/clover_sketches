#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "sequence/event.hpp"

// clang-format off
struct pattern_meta {
    // done
    std::vector<event_meta_sq> patterns_kick{
            {.start_time = 0,  .pattern_index = 0},
            // {.start_time = 1,  .pattern_index = 1},
            // {.start_time = 2,  .pattern_index = 0},
            // {.start_time = 3,  .pattern_index = 1},
            // {.start_time = 4,  .pattern_index = 0},
            {.start_time = 32, .pattern_index = 1},
    };

    // done
    std::vector<event_meta_sq> patterns_bass{
            // {.start_time = 0,  .pattern_index = 0},
            // {.start_time = 1,  .pattern_index = 1},
            // {.start_time = 2,  .pattern_index = 0},
            // {.start_time = 3,  .pattern_index = 1},
            // {.start_time = 4,  .pattern_index = 0},
            // {.start_time = 32, .pattern_index = 1},
            {.start_time = 0,   .pattern_index = 0},
            {.start_time = 32,  .pattern_index = 1},
            {.start_time = 177, .pattern_index = 0},
            {.start_time = 185, .pattern_index = 1},
    };

    std::vector<event_meta_sq> patterns_hh1{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 2, .pattern_index = 1}, // only for troubleshooting the frsq
    };

    std::vector<event_meta_sq> patterns_hh2{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 2, .pattern_index = 1}, // only for troubleshooting the frsq
    };

    std::vector<event_meta_sq> patterns_hh3{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 2, .pattern_index = 1}, // only for troubleshooting the frsq
    };

    // done
    std::vector<event_meta_sq> patterns_ride{
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 81,  .pattern_index = 1},
        {.start_time = 89,  .pattern_index = 0},
        {.start_time = 97,  .pattern_index = 1},
        {.start_time = 127, .pattern_index = 0},
        {.start_time = 169, .pattern_index = 1},
        {.start_time = 182, .pattern_index = 0},
    };

    // done
    std::vector<event_meta_sq> patterns_chord {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 57,  .pattern_index = 1},
        {.start_time = 64,  .pattern_index = 3},
        {.start_time = 65,  .pattern_index = 1},
        {.start_time = 113, .pattern_index = 0},
        {.start_time = 157, .pattern_index = 2},
        {.start_time = 161, .pattern_index = 3},
        {.start_time = 185, .pattern_index = 1},
        {.start_time = 209, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_a1 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 42,  .pattern_index = 1},
        {.start_time = 97,  .pattern_index = 0},
        {.start_time = 125, .pattern_index = 3},
        {.start_time = 161, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_a2 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 89,  .pattern_index = 2},
        {.start_time = 133, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_b1 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 41,  .pattern_index = 1},
        {.start_time = 97,  .pattern_index = 0},
        {.start_time = 125, .pattern_index = 3},
        {.start_time = 161, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_b2 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 89,  .pattern_index = 2},
        {.start_time = 133, .pattern_index = 0},
    };

    // done
    std::vector<event_meta_sq> patterns_pad {
        {.start_time = 0,   .pattern_index = 1},
        {.start_time = 17,  .pattern_index = 0},
        {.start_time = 65,  .pattern_index = 1},
        {.start_time = 97,  .pattern_index = 0},
        {.start_time = 153, .pattern_index = 1},
        {.start_time = 185, .pattern_index = 0},
    };
};
// clang-format on
