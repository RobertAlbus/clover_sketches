#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"

using namespace clover::dsp;

float fb_coefficient(float reduction_db, float delay_line_duration_samples, float decay_duration_samples);

struct fdn_section {
    fdn_section(size_t delay_size_max, float delay_samples) : fdl_tap{delay_samples}, fdl{delay_size_max} {
    }

    float prev_feedback{0};
    float fb_coefficient{0};
    float fdl_tap;
    fdl_lagrange fdl;
    filter lpf;
    filter hpf;
};

struct fdn_4 {
    float fs;
    fdn_section sections[4];
    fdn_4(float fs);

    float tick(float x);
};
