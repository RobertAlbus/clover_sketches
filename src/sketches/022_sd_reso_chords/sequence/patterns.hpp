#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "lib/sq/frsq.hpp"
#include "lib/env_bp/env_bp.hpp"
#include "sequence/event.hpp"

template <frsq_data_base_000 event_t>
struct pattern_t {
    double duration_bar;
    double duration_rel;
    std::vector<event_t> pattern;
};

namespace pattern {

extern std::vector<pattern_t<event>> kick;
extern std::vector<pattern_t<event_midi>> chord;

}  // namespace pattern

namespace arrangement {

// WARNING: this won't work for now
// - env_bp in composition arn't aware of global time.
// - meta frsq can handle different start times, but not
//   yet propagating downward to instrument frsqs
extern double playback_start;

extern std::vector<event_meta_sq> kick;
extern std::vector<event_meta_sq> chord;
extern std::vector<event> bar;

extern std::vector<bp_event_000> bp_env_kick_hp;
extern std::vector<bp_event_000> bp_env_kick_verb_send;

}  // namespace arrangement
