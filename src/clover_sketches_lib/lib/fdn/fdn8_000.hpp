#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <string>

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"

struct fdn8_props_000 {
    std::array<float, 8> taps;
    float fb_gain;
    float lpf_cut;
    float lpf_res;
    float hpf_cut;
    float hpf_res;

    fdn8_props_000 taps_mult(float coeff);
    fdn8_props_000 taps_add(float add);

    std::string to_str();
};

struct fdn8_section_000 {
    fdn8_section_000(float fs);

    float fdl_tap = 4;
    float fb_coef = 0;
    float cut_lpf = 20000;
    float res_lpf = 0.707;
    float cut_hpf = 20;
    float res_hpf = 0.707;

    float fs;
    float prev_feedback{0};
    clover::dsp::fdl_lagrange fdl;
    clover::dsp::filter lpf;
    clover::dsp::filter hpf;

    void set_lpf(float f0, float q);
    void set_hpf(float f0, float q);
    void set_time(float taps);
    void set_fb_coeff(float fb);
};

struct fdn8_000 {
    float fs;

    // THIS IS A HACK.
    // flag to reconfigure internal components every sample
    bool has_gui;

    fdn8_props_000 props;
    std::array<fdn8_section_000, 8> sections;

    // WARNING: use something newer e.g. fdn8_023
    fdn8_000(float fs, const fdn8_props_000& props, bool has_gui = true);
    void patch(fdn8_props_000 props);

    void update_from_props();
    float tick(float x);
    float process(float x);
};
