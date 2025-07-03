#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <string>

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"

struct fdn8_props_023 {
    std::array<float, 8> taps;
    float fb_gain;
    float lpf_cut;
    float lpf_res;
    float hpf_cut;
    float hpf_res;

    fdn8_props_023 taps_mult(float coeff);
    fdn8_props_023 taps_add(float add);

    std::string to_str();
};

struct fdn8_023 {
    float fs;

    fdn8_props_023 props;

    std::array<float, 8> fbs{0, 0, 0, 0, 0, 0, 0, 0};
    std::array<clover::dsp::fdl_lagrange, 8> fdls{
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000},
        clover::dsp::fdl_lagrange{48000}};
    const float max_idx = 47997;
    std::array<clover::dsp::filter, 8> lpfs;
    std::array<clover::dsp::filter, 8> hpfs;

    void set_lpf(float f0);
    void set_lpf(float f0, float q);
    void set_hpf(float f0);
    void set_hpf(float f0, float q);

    fdn8_023(float fs, const fdn8_props_023& props);
    void patch(fdn8_props_023 props);

    float tick(float x);
};
