// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <vector>

#include "patterns.hpp"

arrangement::arrangement() {
    playback_start = 0;

    kick = {
        //
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 28, .pattern_index = 0},
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
        {.start_time = 28, .pattern_index = 0},
    };
    hh2 = {
        //
        {.start_time = 0, .pattern_index = 1},
    };

    chord = {
        //
        {.start_time = 0, .pattern_index = 1},
        {.start_time = 28, .pattern_index = 2},
    };

    bass = {
        //
        {.start_time = 0, .pattern_index = 1},
    };

    // TODO
    // - patterns should be a constructable object
    // - use ctx to configure the duration of the print statement
    bar = std::views::iota(0, 32) |
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
