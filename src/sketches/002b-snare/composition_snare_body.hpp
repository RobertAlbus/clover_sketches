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

// repurposed version of 01_kick
struct snare_body {
    float fs     = 48000;
    int duration = 4 * 60 * 48000;

    oscillator kick_osc;
    env_adsr kick_adsr_gain;
    env_adsr kick_adsr_pitch;

    clover_float pitch_fundamental;
    clover_float pitch_peak;
    clover_float pitch_range;

    filter filt;
    env_adsr kick_adsr_cutoff;
    clover_float cutoff_fundamental;
    clover_float cutoff_peak;
    clover_float cutoff_range;

    int_fast64_t counter_on;
    int_fast64_t counter_off;

    struct ctor_args {
        clover_float pitch_fundamental;
        clover_float pitch_peak;
        clover_float cutoff_fundamental;
        clover_float cutoff_peak;
        clover_float adsr_mult = 1;
        int_fast64_t counter_on;
        int_fast64_t counter_off;
    };
    snare_body(ctor_args args) : kick_osc(fs) {
        kick_osc.freq(pitch_fundamental);
        kick_osc.phase(0);

        kick_adsr_cutoff.set(1, 2000 * args.adsr_mult, 0, 2000);
        kick_adsr_pitch.set(1, 2000 * args.adsr_mult, 0, 1);
        kick_adsr_gain.set(1, 5500 * args.adsr_mult, 0, 1000);

        filt.m_coeffs = lpf(fs, cutoff_fundamental, 1);

        pitch_fundamental  = args.pitch_fundamental;
        pitch_peak         = args.pitch_peak;
        cutoff_fundamental = args.cutoff_fundamental;
        cutoff_peak        = args.cutoff_peak;

        pitch_range  = sketch::octave_difference_by_frequency(pitch_fundamental, pitch_peak);
        cutoff_range = sketch::octave_difference_by_frequency(pitch_fundamental, pitch_peak);

        counter_on  = args.counter_on;
        counter_off = args.counter_off;
    }

    clover_float tick(int_fast64_t counter) {
        if (counter == counter_on) {
            kick_osc.phase(0);
            kick_adsr_cutoff.key_on();
            kick_adsr_pitch.key_on();
            kick_adsr_gain.key_on();

        } else if (counter == counter_off) {
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

        float kick_pitch = frequency_by_octave_difference(pitch_fundamental, pitch_range * kick_pitch_env);
        kick_osc.freq(kick_pitch);

        float kick_cutoff =
                frequency_by_octave_difference(cutoff_fundamental, cutoff_range * kick_cutoff_env);
        filt.m_coeffs = lpf(fs, kick_cutoff, 1);

        return kick_signal;
    };
};
