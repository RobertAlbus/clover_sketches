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

struct kick_props_038 {
    float trim;
    float drive;

    // ADSR configurations using adsr_values structs
    adsr_values amp_adsr;
    adsr_values pitch_adsr;
    adsr_values cut_adsr;

    float filt_q;
    float pitch_fundamental;
    float cut_fundamental;
    float pitch_range;
    float cut_range;

    std::string to_str();
};

struct kick_drum_038 {
    oscillator kick_osc;
    filter filt;

    kick_props_038 props;

    env_adsr adsr_amp;
    env_adsr adsr_pitch;
    env_adsr adsr_cut;

    float fs = 0;
    kick_drum_038(float fs, const kick_props_038& new_props);
    void patch(kick_props_038 props);
    float tick();
    void key_on();
    void key_off();
};
