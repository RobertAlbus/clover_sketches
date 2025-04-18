// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stdexcept>

#include "bar_counter.hpp"

bar_counter::bar_counter(double duration_samples, double duration_bars, bool should_loop)
    : duration_samples{duration_samples},
      duration_bars{duration_bars},
      should_loop{should_loop},
      time_global{0},
      progress_global{0} {
    if (duration_samples < 1)
        throw std::runtime_error("bar_counter duration_samples < 1");
}

void bar_counter::tick() {
    if (!should_loop && time_global >= duration_samples)
        return;

    if (time_global >= duration_samples)
        time_global -= duration_samples;

    progress_global = time_global / duration_samples;
    progress_bars   = progress_global * duration_bars;

    time_global += 1;
}
