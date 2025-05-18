#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "fdn_section.hpp"

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

struct fdn8_000 {
    float fs;

    /*
    TODO: remove this hack.
    - has_gui is a flag to reconfigure internal components every sample
        - needed when state is changed, e.g. from GUI
        - could also programmatically change the state
    - is there a way for the GUI to own the responsibility of calling the update?
        - ^ that is not a good architecture but reasonable for prototyping
    */
    bool has_gui;
    fdn8_props_000 props;
    std::array<fdn_section_012, 8> sections;

    fdn8_000(float fs, const fdn8_props_000& props, bool has_gui = true);
    void patch(fdn8_props_000 props);

    void update_from_props();
    float tick(float x);
    float process(float x);
};
