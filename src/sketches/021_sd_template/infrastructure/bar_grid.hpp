#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

struct bar_grid {
    float fs;
    double samples_per_bar;

    bar_grid(float fs, double bpm, double beats_per_bar = 4);

    double bars_to_samples(double bars);
    double samples_to_bars(double samples);
};
