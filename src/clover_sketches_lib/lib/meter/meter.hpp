#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/env_bp/env_bp.hpp"

extern std::array<bp_event_000, 3> peak_hold_env;

struct meter_peak_032 {
    float value = 0;
    float scale = 0;
    env_bp_000 peak_env;

    meter_peak_032() = delete;
    meter_peak_032(float fs, float hold_ms);

    meter_peak_032(float hold_samples);

    void set_peak_hold(float samples);

    void set_peak_hold(float fs, float hold_ms);

    void tick(float x);
};

struct meter_rms_032 {
    float alpha       = 0.0f;
    float value       = 0.0f;
    float squared_ema = 0.0f;

    meter_rms_032() = delete;
    meter_rms_032(float samples);
    meter_rms_032(float fs, float window_ms = 300.0f);

    void set_alpha(float fs, float ms);
    void set_alpha(float samples);

    void tick(float x);
};

struct meter_gain_mono_032 {
    meter_gain_mono_032() = delete;
    meter_gain_mono_032(float fs);

    meter_peak_032 peak;
    meter_peak_032 peak_hold;
    meter_rms_032 rms;

    void tick(float x);
};

struct meter_gain_stereo_032 {
    meter_gain_stereo_032() = delete;
    meter_gain_stereo_032(float fs);

    meter_gain_mono_032 meter_L;
    meter_gain_mono_032 meter_R;

    void tick(float L, float R);
};
