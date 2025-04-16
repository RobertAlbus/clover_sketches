#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "sequence/event.hpp"

namespace pattern {

extern std::vector<std::vector<event>> kick;
extern std::vector<std::vector<event_midi>> chord;

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

}  // namespace arrangement
