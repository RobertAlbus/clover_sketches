#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "lib/env_bp/env_bp.hpp"
#include "lib/sq/frsq_000.hpp"  // for frsq_data_base_000

#include "sequence/event.hpp"

template <frsq_data_base_000 event_t>
struct pattern_t {
    double duration_bar;
    double duration_rel;
    std::vector<event_t> pattern;
};

// OPEN QUESTION:
// do I prefer creating patterns
// - in functions (patterns struct) (could also use free functions)
// - in ctor      (arrangement struct)

struct patterns {
    std::vector<pattern_t<event>> sc_pump                         = create_pattern_sc_pump();
    std::vector<pattern_t<event>> kick                            = create_pattern_kick();
    std::vector<pattern_t<event>> snare                           = create_pattern_snare();
    std::vector<pattern_t<event>> ride                            = create_pattern_ride();
    std::vector<pattern_t<event>> hh1                             = create_pattern_hh1();
    std::vector<pattern_t<event>> hh2                             = create_pattern_hh2();
    std::vector<pattern_t<event>> hh3                             = create_pattern_hh3();
    std::vector<pattern_t<event_midi>> chord                      = create_pattern_chord();
    std::vector<pattern_t<event_midi>> bass                       = create_pattern_bass();
    std::vector<pattern_t<event_fdn_8_tap_tuning>> chord_fdn_taps = create_pattern_chord_fdn_taps();

    std::vector<pattern_t<event>> create_pattern_sc_pump();
    std::vector<pattern_t<event>> create_pattern_kick();
    std::vector<pattern_t<event>> create_pattern_snare();
    std::vector<pattern_t<event>> create_pattern_ride();
    std::vector<pattern_t<event>> create_pattern_hh1();
    std::vector<pattern_t<event>> create_pattern_hh2();
    std::vector<pattern_t<event>> create_pattern_hh3();
    std::vector<pattern_t<event_midi>> create_pattern_chord();
    std::vector<pattern_t<event_midi>> create_pattern_bass();
    std::vector<pattern_t<event_fdn_8_tap_tuning>> create_pattern_chord_fdn_taps();
};

struct arrangement {
    arrangement();

    // WARNING: playback_start won't work for now
    // - env_bp in composition aren't aware of global time.
    // - meta frsq can handle different start times (still true?), but not
    //   yet propagating downward to instrument frsqs
    double playback_start;

    std::vector<event_meta_sq> sc_pump;
    std::vector<event_meta_sq> kick;
    std::vector<event_meta_sq> snare;
    std::vector<event_meta_sq> ride;
    std::vector<event_meta_sq> hh1;
    std::vector<event_meta_sq> hh2;
    std::vector<event_meta_sq> hh3;
    std::vector<event_meta_sq> chord;
    std::vector<event_meta_sq> chord_fdn_taps;
    std::vector<event_meta_sq> bass;
    std::vector<event> bar;

    std::vector<bp_event_000> bp_env_kick_hp;
    std::vector<bp_event_000> bp_env_kick_verb_send;
};
