#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "fdn_section.hpp"

float fb_coefficient(float reduction_db, float delay_line_duration_samples, float decay_duration_samples);

struct fdn_8_props_019 {
    std::array<float, 8> taps;
    float fb_gain;
    float lpf_cut;
    float lpf_res;
    float hpf_cut;
    float hpf_res;

    std::string to_str();
};

extern fdn_8_props_019 fdn_patch;

struct fdn_8_019 {
    float fs;
    bool has_gui;
    fdn_8_props_019 props;
    std::array<fdn_section_012, 8> sections;

    fdn_8_019(float fs, const fdn_8_props_019& props, bool has_gui = false);
    void patch(fdn_8_props_019 props);

    void update_from_props();
    float tick(float x);
    float process(float x);
};
