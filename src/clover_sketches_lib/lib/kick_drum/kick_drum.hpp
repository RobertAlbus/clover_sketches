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

struct kick_props_000 {
    float trim;
    float drive;

    float amp_a;
    float amp_d;
    float amp_s;
    float amp_r;

    float cut_a;
    float cut_d;
    float cut_s;
    float cut_r;
    float filt_q;

    float pitch_a;
    float pitch_d;
    float pitch_s;
    float pitch_r;

    float pitch_fundamental;
    float cut_fundamental;

    float pitch_range;
    float cut_range;

    std::string to_str();
};

struct kick_drum_000 {
    oscillator kick_osc;
    filter filt;

    kick_props_000 props;

    env_adsr adsr_amp;
    env_adsr adsr_pitch;
    env_adsr adsr_cut;

    float fs = 0;
    kick_drum_000(float fs, const kick_props_000& new_props);
    void patch(kick_props_000 props);
    float tick();
    void key_on();
    void key_off();
};
