#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "fdn_section.hpp"

float fb_coefficient(float reduction_db, float delay_line_duration_samples, float decay_duration_samples);

struct fdn_8_props_012 {
    std::array<float, 8> taps;  //{4, 4, 4, 4, 4, 4, 4, 4};
    float fb_gain;              // = 0;
    float lpf_cut;              // = 11621.206;
    float lpf_res;              // = 0.707;
    float hpf_cut;              // = 20;
    float hpf_res;              // = 0.707;

    std::string to_str();
};

extern fdn_8_props_012 fdn_patch;

struct fdn_8_012 {
    float fs;
    fdn_8_props_012 props;
    std::array<fdn_section_012, 8> sections;

    fdn_8_012(float fs, const fdn_8_props_012& props);
    void patch(const fdn_8_props_012& props);

    float tick(float x);
    float process(float x);
};
