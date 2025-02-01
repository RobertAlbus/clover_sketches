#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"

using namespace clover;
using namespace dsp;

struct kick_drum {
    float fs = 0;
    kick_drum(float fs);
    float tick();
    void key_on();
    void key_off();

    oscillator kick_osc;
    env_adsr adsr_amp;

    env_adsr adsr_pitch;
    float pitch_fundamental = 50;
    float pitch_peak        = 200;
    float pitch_range =
            clover::octave_difference_by_frequency(pitch_fundamental, pitch_peak);

    filter filt;
    env_adsr adsr_cut;
    float cutoff_fundamental = 100;
    float cutoff_peak        = 200;
    float cutoff_range_octaves =
            clover::octave_difference_by_frequency(pitch_fundamental, pitch_peak);
};
