#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"

#include "settable.hpp"

using namespace clover;
using namespace dsp;

struct kick_props {
    settable gain;
    settable drive;

    settable amp_a;
    settable amp_d;
    settable amp_s;
    settable amp_r;

    settable cut_a;
    settable cut_d;
    settable cut_s;
    settable cut_r;
    settable filt_q;

    settable pitch_a;
    settable pitch_d;
    settable pitch_s;
    settable pitch_r;

    settable pitch_fundamental;
    settable pitch_peak;
    settable cut_fundamental;
    settable cut_peak;

    float pitch_range;  // computed from two props that are already smoothed
    float cut_range;    // computed from two props that are already smoothed

    void tick();
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
    kick_drum(float fs);
    void patch(kick_props& props);
    float tick();
    void update_state();
    void key_on();
    void key_off();
};
