#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

struct bar_grid {
    float fs;
    double samples_per_bar;

    double bpm;
    double duration_bars;
    bool should_loop;

    // int_fast64_t duration = int_fast64_t(sp_bar * duration_bars) + 1;

    bar_grid(float fs, double bpm, double duration_bars, bool should_loop, double beats_per_bar = 4);

    double bars_to_samples(double bars);
    double samples_to_bars(double samples);
    double duration_samples();
};
