// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"

#import "kick.hpp"

using namespace clover;
using namespace dsp;

kick_drum::kick_drum(clover_float fs) : fs(fs), kick_osc(fs) {
    kick_osc.freq(pitch_fundamental);
    kick_osc.phase(0);

    adsr_cut.set(1, 2000, 0, 2000);
    adsr_pitch.set(1, 3000, 0, 1);
    adsr_amp.set(1, 5000, 0.1, 1000);

    filt.m_coeffs = lpf(fs, cutoff_fundamental, 1);
}

void kick_drum::key_on() {
    kick_osc.phase(0);
    adsr_cut.key_on();
    adsr_pitch.key_on();
    adsr_amp.key_on();
}
void kick_drum::key_off() {
    adsr_cut.key_off();
    adsr_pitch.key_off();
    adsr_amp.key_off();
}

clover_float kick_drum::tick() {
    clover_float kick_osc_signal = kick_osc.tick();
    clover_float kick_cutoff_env = adsr_cut.tick();
    clover_float kick_gain_env   = adsr_amp.tick();
    clover_float kick_pitch_env  = adsr_pitch.tick();

    clover_float kick_signal = kick_osc_signal * kick_gain_env;
    kick_signal              = std::tanh(kick_signal * 1.4f);
    kick_signal              = filt.tick(kick_signal);

    float kick_pitch =
            frequency_by_octave_difference(pitch_fundamental, pitch_range * kick_pitch_env);
    kick_osc.freq(kick_pitch);

    float kick_cutoff = frequency_by_octave_difference(
            cutoff_fundamental, cutoff_range_octaves * kick_cutoff_env);
    filt.m_coeffs = lpf(fs, kick_cutoff, 1);

    return kick_signal;
};
