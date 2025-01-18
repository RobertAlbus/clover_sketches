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
    clover_float fs = 0;
    kick_drum(clover_float fs);
    clover_float tick();
    void key_on();
    void key_off();

    oscillator kick_osc;
    env_adsr kick_adsr_gain;
    env_adsr kick_adsr_pitch;

    clover_float kick_osc_pitch_fundamental = 50;
    clover_float kick_osc_pitch_peak        = 200;
    clover_float kick_osc_pitch_range =
            clover::octave_difference_by_frequency(kick_osc_pitch_fundamental, kick_osc_pitch_peak);

    filter filt;
    env_adsr kick_adsr_cutoff;
    clover_float kick_osc_cutoff_fundamental = 100;
    clover_float kick_osc_cutoff_peak        = 200;
    clover_float kick_osc_cutoff_range =
            clover::octave_difference_by_frequency(kick_osc_pitch_fundamental, kick_osc_pitch_peak);
};
