#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <vector>

#include "sequence/event.hpp"

// clang-format off
struct pattern_meta {
    // WARNING: this won't work for now
    // - env_bp in composition arn't aware of global time.
    // - meta sequencers only initiate patterns at relative time 0.
    double PLAYBACK_START = 0; 
    
    // done
    std::vector<event_meta_sq> patterns_kick{
            {.start_time = 0,   .pattern_index = 1},
            {.start_time = 129, .pattern_index = 0},
            {.start_time = 133, .pattern_index = 1},
            {.start_time = 224, .pattern_index = 0},
    };

    // done
    std::vector<event_meta_sq> patterns_bass{
            {.start_time = 0,   .pattern_index = 0},
            {.start_time = 32,  .pattern_index = 1},
            {.start_time = 97,  .pattern_index = 1},
            {.start_time = 133, .pattern_index = 0},
            {.start_time = 135, .pattern_index = 1},
            {.start_time = 177, .pattern_index = 0},
            {.start_time = 185, .pattern_index = 1},
            {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_hh1{
        {.start_time = 0,  .pattern_index = 0},
        {.start_time = 32, .pattern_index = 1},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_hh2{
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_hh3{
        {.start_time = 0,   .pattern_index = 1},
        {.start_time = 65,  .pattern_index = 1},
        {.start_time = 97,  .pattern_index = 2},
        {.start_time = 133, .pattern_index = 0},
        {.start_time = 153, .pattern_index = 2},
        {.start_time = 177, .pattern_index = 0},
        {.start_time = 185, .pattern_index = 2},
        {.start_time = 224, .pattern_index = 0},
    };

    // done
    std::vector<event_meta_sq> patterns_ride{
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 81,  .pattern_index = 1},
        {.start_time = 89,  .pattern_index = 0},
        {.start_time = 97,  .pattern_index = 1},
        {.start_time = 113, .pattern_index = 0},
        {.start_time = 169, .pattern_index = 1},
        {.start_time = 185, .pattern_index = 0},
        {.start_time = 224, .pattern_index = 0},
    };

    // done
    std::vector<event_meta_sq> patterns_chord {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 57,  .pattern_index = 1},
        {.start_time = 64,  .pattern_index = 3},
        {.start_time = 65,  .pattern_index = 1},
        {.start_time = 129, .pattern_index = 0},
        {.start_time = 157, .pattern_index = 2},
        {.start_time = 161, .pattern_index = 3},
        {.start_time = 209, .pattern_index = 2},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_a1 {
        {.start_time = 0,   .pattern_index = 2},
        {.start_time = 32,  .pattern_index = 0},
        {.start_time = 42,  .pattern_index = 2},
        {.start_time = 97,  .pattern_index = 3},
        {.start_time = 129, .pattern_index = 2},
        {.start_time = 137, .pattern_index = 1},
        {.start_time = 161, .pattern_index = 2},
        {.start_time = 185, .pattern_index = 0},
        {.start_time = 193, .pattern_index = 2},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_a2 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_b1 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event_meta_sq> patterns_lead_b2 {
        {.start_time = 0,   .pattern_index = 0},
        {.start_time = 224, .pattern_index = 0},
    };

    // done
    std::vector<event_meta_sq> patterns_pad {
        {.start_time = 0,   .pattern_index = 1},
        {.start_time = 30,  .pattern_index = 0},
        {.start_time = 32,  .pattern_index = 1},
        {.start_time = 65,  .pattern_index = 1},
        {.start_time = 97,  .pattern_index = 0},
        {.start_time = 137, .pattern_index = 1},
        {.start_time = 193, .pattern_index = 0},
        {.start_time = 224, .pattern_index = 0},
    };

    std::vector<event> timing_range = std::views::iota(0, 227)
                    | std::views::transform([](int i) { return event{.start_time = double(i)}; })
                    | std::ranges::to<std::vector>();

};
// clang-format on
