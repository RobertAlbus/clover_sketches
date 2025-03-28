#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
using namespace clover;
using namespace dsp;

struct kick_props {
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
    float pitch_peak;
    float cut_fundamental;
    float cut_peak;

    float pitch_range;  // computed from two props that are already smoothed
    float cut_range;    // computed from two props that are already smoothed

    std::string to_str();
};

struct kick_drum {
    oscillator kick_osc;
    filter filt;

    kick_props props;

    env_adsr adsr_amp;
    env_adsr adsr_pitch;
    env_adsr adsr_cut;

    float fs = 0;
    kick_drum(float fs, kick_props& new_props);
    void patch(kick_props& props);
    float tick();
    void key_on();
    void key_off();
};
