// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include "sequence/event.hpp"
#include "sequence/notes.h"

std::vector<pattern_t<event_midi>> patterns::create_pattern_chord() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 4,
         .duration_rel = 16.,
         .pattern =
             {
                 //
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::C4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::Eb4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::G4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::Bb4},
                 {.start_time = 1.75, .duration = 0.75, .note = -17 + note::F4},

                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::C4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::Eb4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::G4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::Bb4},
                 {.start_time = 5.75, .duration = 0.75, .note = -17 + note::F4},

                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::C4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::Eb4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::G4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::Bb4},
                 {.start_time = 9.75, .duration = 0.75, .note = -17 + note::F4},

                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 13.75, .duration = 0.25, .note = -17 + note::F4},

                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 14.25, .duration = 0.25, .note = -17 + note::F4},
             }},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //

                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 1.75, .duration = 0.25, .note = -17 + note::F4},

                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::C4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::Eb4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::G4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::Bb4},
                 {.start_time = 2.25, .duration = 0.25, .note = -17 + note::F4},
             }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
             {
                 //
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::C4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::Eb4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::G4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::Bb4},
                 {.start_time = 5.5, .duration = 0.75, .note = -12 + note::F4},
             }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
             {
                 //
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::C4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::Eb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::G4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::Bb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -24 + note::F4},
             }},
        {.duration_bar = 2,
         .duration_rel = 8.,
         .pattern =
             {
                 //
                 {.start_time = 1.5, .duration = 0.75, .note = note::C4},
                 {.start_time = 1.5, .duration = 0.75, .note = note::Eb4},
                 {.start_time = 1.5, .duration = 0.75, .note = note::G4},
                 {.start_time = 1.5, .duration = 0.75, .note = note::Bb4},
                 {.start_time = 1.5, .duration = 0.75, .note = note::F4},

                 //
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::C4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::Eb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::G4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::Bb4},
                 {.start_time = 1.5, .duration = 0.75, .note = -12 + note::F4},
             }},
    };
}

std::vector<pattern_t<event_midi>> patterns::create_pattern_bass() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 // clang-format off
                 {.start_time = 0.5, .duration = 1, .note = note::G4 - 24},
                //  {.start_time = 3,   .duration = 0.95, .note = note::G4 - 24},
                 // clang-format on
             }},
    };
}

namespace {
std::array<float, 8> lerp_taps(std::array<float, 8> a_taps, std::array<float, 8> b_taps, float t) {
    std::array<float, 8> result;
    for (auto [i, a, b] : std::views::zip(std::views::iota(0, 8), a_taps, b_taps)) {
        result[i] = std::lerp(a, b, t);
    }
    return result;
}
}  // namespace
std::vector<pattern_t<event_fdn_8_tap_tuning>> patterns::create_pattern_chord_fdn_taps() {
    // walk through taps[0] = 1012, 1022, 1033, 103
    constexpr std::array<float, 8> taps_a = {
        1012, 287.87997, 290.91763, 300.22708, 698.1039, 713.03784, 150.73712, 158.75778};
    constexpr std::array<float, 8> taps_b = {
        1022, 287.87997, 290.91763, 300.22708, 698.1039, 713.03784, 150.73712, 158.75778};
    constexpr std::array<float, 8> taps_c = {
        1033, 287.87997, 290.91763, 300.22708, 698.1039, 713.03784, 150.73712, 158.75778};
    constexpr std::array<float, 8> taps_d = {
        103, 287.87997, 290.91763, 300.22708, 698.1039, 713.03784, 150.73712, 158.75778};
    constexpr std::array<float, 8> taps_big_wash = {
        32238.998, 16120, 870, 435, 217.49998, 108, 54.000004, 27};

    constexpr int transition_duration_bars = 16;
    constexpr int transition_step_count    = 64 * 64 * 4;
    auto a_to_wash = [transition_step_count, taps_a, taps_big_wash](float start_time, float end_time) {
        std::vector<event_fdn_8_tap_tuning> pattern;
        pattern.reserve(transition_step_count);
        for (auto i : std::views::iota(0, transition_step_count)) {
            const float t           = float(i) / float(transition_step_count);
            const double start_time = start_time + (t * (end_time - start_time));
            pattern.emplace_back(
                event_fdn_8_tap_tuning{
                    .start_time = start_time, .duration = 0.1, .taps = lerp_taps(taps_a, taps_big_wash, t)});
        }
        return pattern;
    };

    std::vector<pattern_t<event_fdn_8_tap_tuning>> patterns = {
        {.duration_bar = 1,
         .duration_rel = 4,
         .pattern =
             {
                 //
                 {.start_time = 1.75, .duration = 0.5, .taps = taps_a},
             }},
        {.duration_bar = 1,
         .duration_rel = 4,
         .pattern =
             {
                 //
                 {.start_time = 1.75, .duration = 0.9, .taps = taps_b},
             }},
        {.duration_bar = 1,
         .duration_rel = 4,
         .pattern =
             {
                 //
                 {.start_time = 1.75, .duration = 0.9, .taps = taps_c},
             }},
        {.duration_bar = 1,
         .duration_rel = 4,
         .pattern =
             {
                 //
                 {.start_time = 1.75, .duration = 0.9, .taps = taps_d},
             }},
        {.duration_bar = 1,
         .duration_rel = 4,
         .pattern =
             {
                 //
                 {.start_time = 0, .duration = 0.9, .taps = taps_big_wash},
             }},

        {.duration_bar = 16, .duration_rel = 16, .pattern = a_to_wash(0, 8)},
    };

    // std::println("{}", patterns[5].pattern.size());
    // for (auto& p : patterns[5].pattern) {
    //     std::println(
    //         "{:.4f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f} {:.2f}",
    //         p.start_time,
    //         p.taps[0],
    //         p.taps[1],
    //         p.taps[2],
    //         p.taps[3],
    //         p.taps[4],
    //         p.taps[5],
    //         p.taps[6],
    //         p.taps[7]);
    // }
    return patterns;
}