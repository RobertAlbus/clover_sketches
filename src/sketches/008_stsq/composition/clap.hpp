#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <map>

#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"
#include "clover/dsp/oscillator.hpp"

using namespace clover;
using namespace dsp;

struct trigger_info {
    int start_time;
    float env_duration;
};

struct clap_envelope {
    clover::dsp::env_linear env;
    int duration;
    int count;

    std::map<int, float> triggers;

    clap_envelope(int pulse_count, int spacing, int duration, int duration_final);
    clover_float tick();
    void trigger();
};

struct hand_clap {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator noise_generator;

    filter_2 eqs[3];

    fdl_lagrange_2 delay{96000};

    int_fast64_t counter       = 0;
    int_fast64_t snare_key_on  = 24000;
    int_fast64_t snare_key_off = 33000;

    clap_envelope clap_env;

    hand_clap();

    std::pair<clover_float, clover_float> tick();
};
