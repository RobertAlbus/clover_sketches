#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/float.hpp"

namespace sketch {

auto octave_difference_by_frequency(clover_float base_freq, clover_float target_freq) -> clover_float;
auto semitone_difference_by_frequency(clover_float base_freq, clover_float target_freq) -> clover_float;

}  // namespace sketch