#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/env_bp/env_bp.hpp"

extern std::array<bp_event_000, 3> peak_hold_env;

struct meter_peak {
    float value = 0;
    float scale = 0;
    env_bp_000 peak_env;

    meter_peak() = delete;
    meter_peak(float fs, float hold_ms);

    meter_peak(float hold_samples);

    void set_peak_hold(float samples);

    void set_peak_hold(float fs, float hold_ms);

    void tick(float x);
};

struct meter_rms {
    float alpha       = 0.0f;
    float value       = 0.0f;
    float squared_ema = 0.0f;

    meter_rms() = delete;
    meter_rms(float samples);
    meter_rms(float fs, float window_ms = 300.0f);

    void set_alpha(float fs, float ms);
    void set_alpha(float samples);

    void tick(float x);
};

struct meter_mono {
    meter_mono() = delete;
    meter_mono(float fs);

    meter_peak peak;
    meter_peak peak_hold;
    meter_rms rms;

    void tick(float x);
};

struct meter_stereo {
    meter_stereo() = delete;
    meter_stereo(float fs);

    meter_mono meter_L;
    meter_mono meter_R;

    void tick(float L, float R);
};
