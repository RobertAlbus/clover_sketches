// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"

#include "util/math.hpp"

using namespace clover;
using namespace dsp;

struct kick_drum {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator kick_osc;
    env_adsr kick_adsr_gain;
    env_adsr kick_adsr_pitch;

    clover_float kick_osc_pitch_fundamental = 50;
    clover_float kick_osc_pitch_peak        = 200;
    clover_float kick_osc_pitch_range =
            sketch::octave_difference_by_frequency(kick_osc_pitch_fundamental, kick_osc_pitch_peak);

    filter filt;
    env_adsr kick_adsr_cutoff;
    clover_float kick_osc_cutoff_fundamental = 100;
    clover_float kick_osc_cutoff_peak        = 200;
    clover_float kick_osc_cutoff_range =
            sketch::octave_difference_by_frequency(kick_osc_pitch_fundamental, kick_osc_pitch_peak);

    kick_drum() : kick_osc(fs) {
        kick_osc.freq(kick_osc_pitch_fundamental);
        kick_osc.phase(0);

        kick_adsr_cutoff.set(1, 2000, 0, 2000);
        kick_adsr_pitch.set(1, 3000, 0, 1);
        kick_adsr_gain.set(1, 5000, 0.1, 5000);

        filt.m_coeffs = lpf(fs, kick_osc_cutoff_fundamental, 1);
    }

    clover_float tick(int_fast64_t counter) {
        if (counter == 0 || counter == 24000) {
            kick_osc.phase(0);
            kick_adsr_cutoff.key_on();
            kick_adsr_pitch.key_on();
            kick_adsr_gain.key_on();

        } else if (counter == 10000 || counter == 34000) {
            kick_adsr_cutoff.key_off();
            kick_adsr_pitch.key_off();
            kick_adsr_gain.key_off();
        }

        clover_float kick_osc_signal = kick_osc.tick();
        clover_float kick_cutoff_env = kick_adsr_cutoff.tick();
        clover_float kick_gain_env   = kick_adsr_gain.tick();
        clover_float kick_pitch_env  = kick_adsr_pitch.tick();

        clover_float kick_signal = kick_osc_signal * kick_gain_env;
        kick_signal              = std::tanh(kick_signal * 1.4f);
        kick_signal              = filt.tick(kick_signal);

        float kick_pitch = frequency_by_octave_difference(
                kick_osc_pitch_fundamental, kick_osc_pitch_range * kick_pitch_env);
        kick_osc.freq(kick_pitch);

        float kick_cutoff = frequency_by_octave_difference(
                kick_osc_cutoff_fundamental, kick_osc_cutoff_range * kick_cutoff_env);
        filt.m_coeffs = lpf(fs, kick_cutoff, 1);

        return kick_signal;
    };
};
