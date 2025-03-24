#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"

using namespace clover;
using namespace dsp;

struct cymbal {
    clover_float fs = 0;

    cymbal(clover_float fs);
    void key_on();
    void key_off();
    void set_oscs();
    void set_adsrs();

    clover_float tick();

    static const int num_oscs = 6;
    oscillator osc[num_oscs];
    float osc_freq[num_oscs]{199, 215, 253, 307, 329, 405};

    env_adsr adsr_amp;
    env_adsr adsr_cut;

    filter high_pass;
    filter band_pass;

    float hpf_f0 = 3000;
    float hpf_f1 = 8000;
    float hpf_Q  = 1;
    float bpf_f0 = 6000;
    float bpf_Q  = 1.3;

    float amp_a = 10;
    float amp_d = 1500;
    float amp_s = 0;
    float amp_r = 10;

    float cut_a = 10;
    float cut_d = 100;
    float cut_s = 0;
    float cut_r = 10;
};
