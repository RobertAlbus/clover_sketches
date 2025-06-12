#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

struct bar_grid_029 {
    float fs;
    double samples_per_bar;

    double bpm;
    double duration_bars;
    bool should_loop;

    bar_grid_029(float fs, double bpm, double duration_bars, double beats_per_bar = 4);

    double bars_to_samples(double bars);
    double samples_to_bars(double samples);
    double duration_samples();
};
