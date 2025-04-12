#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "sequence/event.hpp"
#include "sequence/notes.h"

// clang-format off
struct pattern_synth {
    std::vector<std::vector<event_midi>> patterns_chord {
        {},
        {
            {.start_time = 1.5, .duration = 0.5, .note = note::E3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Gs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::B3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Cs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Fs3 + 12},
        },
        {
            {.start_time = 3, .duration = 0.5, .note = note::Fs3},
            {.start_time = 3, .duration = 0.5, .note = note::B3},
            {.start_time = 3, .duration = 0.5, .note = note::Cs3},
            {.start_time = 3, .duration = 0.5, .note = note::E3},
            {.start_time = 3, .duration = 0.5, .note = note::Gs3 + 12},
        },
        {
            {.start_time = 1.5, .duration = 0.5, .note = note::E3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Gs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::B3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Cs3},
            {.start_time = 1.5, .duration = 0.5, .note = note::Fs3 + 12},

            {.start_time = 3, .duration = 0.5, .note = note::Fs3},
            {.start_time = 3, .duration = 0.5, .note = note::B3},
            {.start_time = 3, .duration = 0.5, .note = note::Cs3},
            {.start_time = 3, .duration = 0.5, .note = note::E3},
            {.start_time = 3, .duration = 0.5, .note = note::Gs3 + 12},
        },

    };

    std::vector<std::vector<event_midi>> patterns_lead_a {
        {},
        {
            {.start_time = 0,  .duration = 0.2, .note = note::E3},
            {.start_time = 1,  .duration = 0.2, .note = note::E3},
            {.start_time = 2,  .duration = 0.2, .note = note::E3},
            {.start_time = 3,  .duration = 0.2, .note = note::E4 + 0.55},
            {.start_time = 4,  .duration = 0.2, .note = note::E3},
            {.start_time = 5,  .duration = 0.2, .note = note::E3},
            {.start_time = 6,  .duration = 0.2, .note = note::E3},
            {.start_time = 7,  .duration = 0.2, .note = note::E3},
            {.start_time = 8,  .duration = 0.2, .note = note::E3},
            {.start_time = 9,  .duration = 0.2, .note = note::E3},
            {.start_time = 10, .duration = 0.2, .note = note::E3},
            {.start_time = 11, .duration = 0.2, .note = note::E3},
            {.start_time = 12, .duration = 0.2, .note = note::E4 + 0.55},
            {.start_time = 13, .duration = 0.2, .note = note::E3},
            {.start_time = 14, .duration = 0.2, .note = note::E3},
            {.start_time = 15, .duration = 0.2, .note = note::E3},
        },
        { // this is still kinda shitty
            {.start_time = 0,  .duration = 0.2, .note = note::E3},
            {.start_time = 1,  .duration = 0.2, .note = note::E3},
            // {.start_time = 2,  .duration = 0.2, .note = note::E3},
            {.start_time = 3,  .duration = 0.2, .note = note::E4 + 0.55},
            {.start_time = 4,  .duration = 0.2, .note = note::E3},
            // {.start_time = 5,  .duration = 0.2, .note = note::E3},
            {.start_time = 6,  .duration = 0.2, .note = note::E3},
            // {.start_time = 7,  .duration = 0.2, .note = note::E3},
            {.start_time = 8,  .duration = 0.2, .note = note::E3},
            {.start_time = 9,  .duration = 0.2, .note = note::E3},
            // {.start_time = 10, .duration = 0.2, .note = note::E3},
            {.start_time = 11, .duration = 0.2, .note = note::E3},
            {.start_time = 12, .duration = 0.2, .note = note::E4 + 0.55},
            // {.start_time = 13, .duration = 0.2, .note = note::E3},
            {.start_time = 14, .duration = 0.2, .note = note::E3},
            // {.start_time = 15, .duration = 0.2, .note = note::E3},
        },
        {

            {.start_time = 0,  .duration = 0.2, .note = note::E3},
            {.start_time = 1,  .duration = 0.2, .note = note::Fs4},
            // {.start_time = 2,  .duration = 0.2, .note = note::E3},
            {.start_time = 3,  .duration = 0.2, .note = note::Fs4 + 0.55},
            {.start_time = 4,  .duration = 0.2, .note = note::B3},
            // {.start_time = 5,  .duration = 0.2, .note = note::E3},
            {.start_time = 6,  .duration = 0.2, .note = note::E3},
            {.start_time = 7,  .duration = 0.2, .note = note::Fs3},
            // {.start_time = 8,  .duration = 0.2, .note = note::E3},
            {.start_time = 9,  .duration = 0.2, .note = note::E3 + 0.55},
            {.start_time = 10, .duration = 0.2, .note = note::Fs4},
            // {.start_time = 11, .duration = 0.2, .note = note::E3},
            {.start_time = 12, .duration = 0.2, .note = note::Fs4 + 0.55},
            {.start_time = 13, .duration = 0.2, .note = note::B3},
            // {.start_time = 14, .duration = 0.2, .note = note::E3},
            // {.start_time = 15, .duration = 0.2, .note = note::Fs3},



            // {.start_time = 0.0,  .duration = 3.0, .note = note::E3},
            // {.start_time = 3.0,  .duration = 2.0, .note = note::Fs4},
            // {.start_time = 5.0,  .duration = 1.0, .note = note::Fs4},
            // {.start_time = 6.0,  .duration = 3.0, .note = note::B3},
            // {.start_time = 9.0,  .duration = 3.0, .note = note::E3},
            // {.start_time = 12.0, .duration = 3.9, .note = note::Fs3},
        },
    };

    std::vector<std::vector<event_midi>> patterns_lead_b {
        {},
        {
            {.start_time = 2,  .duration = 0.2, .note = note::E3},
            {.start_time = 3,  .duration = 0.2, .note = note::E4},// + 0.55},
            {.start_time = 12, .duration = 0.2, .note = note::E4 + 0.55},
            {.start_time = 14, .duration = 0.2, .note = note::E3},
        },
        {
            {.start_time = 0.5,  .duration = 0.2, .note = note::E6},
            {.start_time = 1.0,  .duration = 0.2, .note = note::Fs6},
            {.start_time = 1.5,  .duration = 0.2, .note = note::G6},
            {.start_time = 2.0,  .duration = 0.2, .note = note::E6},
            {.start_time = 2.5,  .duration = 0.2, .note = note::Fs6},
            {.start_time = 3.0,  .duration = 0.2, .note = note::G6},
            {.start_time = 3.5,  .duration = 0.2, .note = note::E6},
            {.start_time = 4.0,  .duration = 0.2, .note = note::Fs6},
            {.start_time = 4.5,  .duration = 0.2, .note = note::G6},
            {.start_time = 5.0,  .duration = 0.2, .note = note::E6},
            {.start_time = 5.5,  .duration = 0.2, .note = note::Fs6},
            {.start_time = 6.0,  .duration = 0.2, .note = note::G6},
            {.start_time = 6.5,  .duration = 0.2, .note = note::E6},
            {.start_time = 7.0,  .duration = 0.2, .note = note::Fs6},
            {.start_time = 7.5,  .duration = 0.2, .note = note::G6},

        },
        {
            {.start_time = 0.5,  .duration = 7.5, .note = note::E3},
            {.start_time = 0.5,  .duration = 1.0, .note = note::Fs3},
            {.start_time = 1.5,  .duration = 5.0, .note = note::G3},
            {.start_time = 5.0,  .duration = 2.0, .note = note::Fs3},
        },
        {
            {.start_time = 0.0,  .duration = 3.0, .note = note::E3},
            {.start_time = 3.0,  .duration = 2.0, .note = note::Fs4},
            {.start_time = 5.0,  .duration = 1.0, .note = note::Fs4},
            {.start_time = 6.0,  .duration = 3.0, .note = note::B3},
            {.start_time = 9.0,  .duration = 3.0, .note = note::E3},
            {.start_time = 12.0, .duration = 3.9, .note = note::Fs3},
        },
        { // 5
            {.start_time = 0.0,  .duration = 6.0, .note = note::E1},
            {.start_time = 6.0,  .duration = 2.0, .note = note::B1},
            {.start_time = 8.0,  .duration = 8.0, .note = note::Fs1},
            // {.start_time = 13.0, .duration = 2.0, .note = note::Fs1},
        },

    };

    std::vector<std::vector<event_midi>> patterns_pad {
        {},
        {
            {.start_time = 0.f,  .duration = 7.9f, .note = note::Fs3},
            {.start_time = 0.f,  .duration = 7.9f, .note = note::As4},
            {.start_time = 0.f,  .duration = 7.9f, .note = note::B4},
            {.start_time = 0.f,  .duration = 7.9f, .note = note::Ds3},
        },
    };
    // clang-format on
};