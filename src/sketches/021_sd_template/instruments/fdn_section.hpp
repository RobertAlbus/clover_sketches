#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"

using namespace clover;
using namespace dsp;

float fb_coefficient(float reduction_db, float delay_line_duration_samples, float decay_duration_samples);

struct fdn_section_012 {
    fdn_section_012(float fs);

    float fdl_tap = 4;
    float fb_coef = 0;
    float cut_lpf = 20000;
    float res_lpf = 0.707;
    float cut_hpf = 20;
    float res_hpf = 0.707;

    float fs;
    float prev_feedback{0};
    fdl_lagrange fdl;
    filter lpf;
    filter hpf;

    void set_lpf(float f0, float q);
    void set_hpf(float f0, float q);
    void set_time(float taps);
    void set_fb_coeff(float fb);
};
