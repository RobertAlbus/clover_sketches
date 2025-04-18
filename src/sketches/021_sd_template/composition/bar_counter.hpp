#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

struct bar_counter {
    double duration_samples;
    double duration_bars;

    double time_global;

    double progress_global;
    double progress_bars;

    bool should_loop;

    bar_counter(double duration_samples, double duration_bars, bool should_loop);
    void tick();
};
