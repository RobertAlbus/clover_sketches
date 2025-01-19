#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <map>

#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/filter.hpp"
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
    hand_clap(clover_float fs);
    std::pair<clover_float, clover_float> tick();
    void key_on();
    void key_off();

    float gain = 1;

    oscillator noise_generator;
    filter_2 eqs[3];

    int duration;
    clap_envelope clap_env;
};
