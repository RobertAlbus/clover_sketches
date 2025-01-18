// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"

using namespace clover;
using namespace dsp;

struct kick_drum {
    kick_drum();
    clover_float tick();

    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    int_fast64_t counter = 0;

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
