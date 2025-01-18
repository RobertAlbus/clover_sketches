#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"

using namespace clover;
using namespace dsp;

struct cymbal {
    float fs = 48000.f;
    int fs_i = int(fs);

    static const int num_oscs = 6;
    oscillator osc[num_oscs]  = {{fs}, {fs}, {fs}, {fs}, {fs}, {fs}};
    float osc_freq[num_oscs]  = {199, 215, 253, 307, 329, 405};

    env_adsr adsr_amp;
    env_adsr adsr_cut;

    filter high_pass;
    filter band_pass;

    float hpf_f0 = 1000;
    float hpf_f1 = 4000;
    float hpf_Q  = 2;
    float bpf_f0 = 6000;
    float bpf_Q  = 1.3;

    float amp_a = 1;
    float amp_d = 48 * 200;
    float amp_s = 0;
    float amp_r = 1;

    float cut_a = 1;
    float cut_d = 48 * 200;
    float cut_s = 0;
    float cut_r = 1;

    cymbal();

    int_fast64_t counter = 0;
    clover_float tick();
};
