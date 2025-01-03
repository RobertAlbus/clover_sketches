// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/float.hpp"

#include "util/math.hpp"

namespace sketch {
auto octave_difference_by_frequency(clover_float base_freq, clover_float target_freq) -> clover_float {
    return std::log2(target_freq / base_freq);
}

auto semitone_difference_by_frequency(clover_float base_freq, clover_float target_freq) -> clover_float {
    return octave_difference_by_frequency(base_freq, target_freq) * 12.f;
}
}  // namespace sketch