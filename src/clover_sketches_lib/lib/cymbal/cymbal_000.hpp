#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"

using namespace clover;
using namespace dsp;

struct cymbal_props_000 {
    static const int num_oscs = 6;

    std::array<float, num_oscs> freqs;
    float hpf_f0           = 100;
    float hpf_fmod_octaves = 8;
    float hpf_Q            = 1;
    float bpf_f0           = 3000;
    float bpf_Q            = 1.3;

    float amp_a = 10;
    float amp_d = 1500;
    float amp_s = 0;
    float amp_r = 10;

    float cut_a = 10;
    float cut_d = 100;
    float cut_s = 0;
    float cut_r = 10;

    // WARNING: heap allocation
    // do not call from the audio thread
    std::string to_str();
};

struct cymbal_000 {
    clover_float fs;
    cymbal_props_000 props;

    cymbal_000(clover_float fs, const cymbal_props_000& new_props);
    void patch(cymbal_props_000 new_props);

    void key_on();
    void key_off();
    void set_oscs();
    void set_adsrs();
    void set_filters();

    clover_float tick();

    std::array<oscillator, cymbal_props_000::num_oscs> oscs;

    env_adsr adsr_amp;
    env_adsr adsr_cut;

    filter high_pass;
    filter band_pass;
};
