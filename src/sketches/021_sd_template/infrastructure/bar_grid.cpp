// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "bar_grid.hpp"

bar_grid::bar_grid(float fs, double bpm, double beats_per_bar) {
    double samples_per_minute = fs * 60;
    double samples_per_beat   = samples_per_minute / bpm;
    samples_per_bar           = samples_per_beat * beats_per_bar;
}

double bar_grid::bars_to_samples(double bars) {
    return bars * samples_per_bar;
}

double bar_grid::samples_to_bars(double samples) {
    return samples / samples_per_bar;
}
