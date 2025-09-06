// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <vector>

#include "patterns.hpp"

arrangement::arrangement() {
    playback_start = 0;

    sc_pump = {
        //
        {.start_time = 0, .pattern_index = 1},
    };

    kick = {
        //
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 72, .pattern_index = 0},
        {.start_time = 80, .pattern_index = 1},
    };

    snare = {
        //
        {.start_time = 0, .pattern_index = 1},
    };

    ride = {
        //
        {.start_time = 0, .pattern_index = 0},
        {.start_time = 12, .pattern_index = 1},
        {.start_time = 16, .pattern_index = 0},
        {.start_time = 24, .pattern_index = 1},
        {.start_time = 28, .pattern_index = 0},
    };
    hh1 = {
        //
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 15, .pattern_index = 2},
        {.start_time = 16, .pattern_index = 1},
        {.start_time = 28, .pattern_index = 0},
    };
    hh2 = {
        //
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 0, .pattern_index = 2},
        {.start_time = 15, .pattern_index = 2},
        {.start_time = 16, .pattern_index = 1},
        {.start_time = 28, .pattern_index = 0},
    };
    hh3 = {
        //
        {.start_time = 0, .pattern_index = 1},
    };

    chord = {
        //
        {.start_time = 0, .pattern_index = 2},
        {.start_time = 16, .pattern_index = 1},
        {.start_time = 64, .pattern_index = 2},
        {.start_time = 80, .pattern_index = 1},
        {.start_time = 144, .pattern_index = 2},  // eventually pattern_index = 3
        {.start_time = 208, .pattern_index = 2},
    };

    constexpr size_t taps_a        = 0;
    constexpr size_t taps_b        = 1;
    constexpr size_t taps_c        = 2;
    constexpr size_t taps_d        = 3;
    constexpr size_t taps_big_wash = 4;

    constexpr size_t taps_aw = 5;  // taps_a -> taps_big_wash
    chord_fdn_taps           = {
        //
        {.start_time = 0, .pattern_index = taps_a},
        {.start_time = 16, .pattern_index = taps_b},
        {.start_time = 32, .pattern_index = taps_c},
        {.start_time = 48, .pattern_index = taps_d},
        {.start_time = 64, .pattern_index = taps_a},

        {.start_time = 68, .pattern_index = taps_aw},
        // {.start_time = 72, .pattern_index = taps_aw_50},
        // {.start_time = 76, .pattern_index = taps_aw_75},

        {.start_time = 80, .pattern_index = taps_big_wash},

        {.start_time = 128, .pattern_index = taps_aw},
        // {.start_time = 132, .pattern_index = taps_aw_50},
        // {.start_time = 136, .pattern_index = taps_aw_25},

        {.start_time = 144, .pattern_index = taps_big_wash},

        {.start_time = 192, .pattern_index = taps_aw},
        // {.start_time = 196, .pattern_index = taps_aw_50},
        // {.start_time = 200, .pattern_index = taps_aw_25},

        {.start_time = 108, .pattern_index = taps_a},
    };

    bass = {
        //
        {.start_time = 0, .pattern_index = 1},
    };

    // TODO
    // - patterns should be a constructable object
    // - use ctx to configure the duration of the print statement
    bar = std::views::iota(0, 224) |
          std::views::transform([](int i) { return event{.start_time = double(i)}; }) |
          std::ranges::to<std::vector>();

    bp_env_kick_hp = {
        /*
        mod range in octaves
        - 0   = 10 hz
        - 3.5 = 113 hz
        - 4   = 160 hz
        - 6   = 640 hz
        */
        {.start = 0, .value = 0},
        {.start = 14, .value = 4}};

    bp_env_kick_verb_send = {
        //
        {.start = 0, .value = 1.0},
        {.start = 1, .value = 1.0}};

}  // namespace arrangement
