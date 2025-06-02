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
// - in functions (patterns struct)
// - in ctor      (arrangement struct)

struct patterns {
    std::vector<pattern_t<event>> kick       = create_pattern_kick();
    std::vector<pattern_t<event>> ride       = create_pattern_ride();
    std::vector<pattern_t<event_midi>> chord = create_pattern_chord();

    std::vector<pattern_t<event>> create_pattern_kick();
    std::vector<pattern_t<event>> create_pattern_ride();
    std::vector<pattern_t<event_midi>> create_pattern_chord();
};

struct arrangement {
    arrangement();

    // WARNING: this won't work for now
    // - env_bp in composition arn't aware of global time.
    // - meta frsq can handle different start times, but not
    //   yet propagating downward to instrument frsqs
    double playback_start;

    std::vector<event_meta_sq> kick;
    std::vector<event_meta_sq> ride;
    std::vector<event_meta_sq> chord;
    std::vector<event> bar;

    std::vector<bp_event_000> bp_env_kick_hp;
    std::vector<bp_event_000> bp_env_kick_verb_send;
};
