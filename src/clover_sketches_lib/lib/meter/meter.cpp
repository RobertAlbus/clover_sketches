// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "lib/env_bp/env_bp.hpp"

#include "meter.hpp"

std::array<bp_event_000, 3> peak_hold_env{
    {//
     {.start = 0, .value = 1},
     {.start = 1, .value = 1},
     {.start = 1.5, .value = 0}}};

meter_peak_032::meter_peak_032(float fs, float hold_ms) {
    peak_env.set_pattern(peak_hold_env);
    peak_env.duration_rel = 2;
    set_peak_hold(fs, hold_ms);
}

meter_peak_032::meter_peak_032(float hold_samples) {
    peak_env.set_pattern(peak_hold_env);
    peak_env.duration_rel = 2;
    set_peak_hold(hold_samples);
}

void meter_peak_032::set_peak_hold(float samples) {
    // absolute duration is for 2 relative units of time.
    // peak hold time is 1 relative unit of 2 relative units.
    peak_env.duration_abs = samples * 2;
}

void meter_peak_032::set_peak_hold(float fs, float hold_ms) {
    set_peak_hold((hold_ms / 1000) * fs);
}

void meter_peak_032::tick(float x) {
    x = std::abs(x);

    if (x > value) {
        scale = x;
        peak_env.key_on();
    }

    value = peak_env.tick() * scale;
}

meter_rms_032::meter_rms_032(float samples) {
    set_alpha(samples);
}
meter_rms_032::meter_rms_032(float fs, float window_ms) {
    set_alpha(fs, window_ms);
}

void meter_rms_032::tick(float x) {
    squared_ema = (alpha * x * x) + ((1.0f - alpha) * squared_ema);
    value       = std::sqrt(squared_ema);
}

void meter_rms_032::set_alpha(float fs, float ms) {
    float samples = (ms / 1000.0f) * fs;
    set_alpha(samples);
}

void meter_rms_032::set_alpha(float samples) {
    alpha = 1.0f - std::exp(-1.0f / samples);
}

meter_gain_mono_032::meter_gain_mono_032(float fs) : peak(fs, 100), peak_hold(fs, 800), rms(fs, 300) {
}

void meter_gain_mono_032::tick(float x) {
    peak.tick(x);
    peak_hold.tick(x);
    rms.tick(x);
}

meter_gain_stereo_032::meter_gain_stereo_032(float fs) : meter_L{fs}, meter_R{fs} {
}

void meter_gain_stereo_032::tick(float L, float R) {
    meter_L.tick(L);
    meter_R.tick(R);
}
