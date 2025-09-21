#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"

#include "lib/_atom/adsr/adsr.hpp"

using namespace clover;
using namespace dsp;

struct cymbal_props_038 {
    static const int num_oscs = 6;

    std::array<float, num_oscs> freqs;
    float hpf_f0           = 100;
    float hpf_fmod_octaves = 8;
    float hpf_Q            = 1;
    float bpf_f0           = 3000;
    float bpf_Q            = 1.3;

    adsr_values amp_adsr = {10, 1500, 0, 10};
    adsr_values cut_adsr = {10, 100, 0, 10};

    // WARNING: heap allocation
    // do not call from the audio thread
    std::string to_str();
};

struct cymbal_038 {
    float fs;
    cymbal_props_038 props;

    cymbal_038(float fs, const cymbal_props_038& new_props);
    void patch(cymbal_props_038 new_props);

    void key_on();
    void key_off();
    void set_oscs();
    void set_adsrs();
    void set_filters();

    float tick();

    std::array<oscillator, cymbal_props_038::num_oscs> oscs;

    env_adsr adsr_amp;
    env_adsr adsr_cut;

    filter high_pass;
    filter band_pass;
};
