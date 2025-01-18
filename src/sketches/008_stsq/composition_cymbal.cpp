// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"

#import "composition_cymbal.hpp"

using namespace clover;
using namespace dsp;

cymbal::cymbal() {
    high_pass.m_coeffs = hpf(fs, hpf_f0, hpf_Q);
    band_pass.m_coeffs = bpf(fs, bpf_f0, bpf_Q);

    adsr_amp.set(amp_a, amp_d, amp_s, amp_r);
    adsr_cut.set(cut_a, cut_d, cut_s, cut_r);

    for (auto i : std::views::iota(0, num_oscs)) {
        osc[i].freq(osc_freq[i]);
        osc[i].waveform = wave_square;
        // osc[i].phase(float(i)/float(num_oscs));
    }
}

clover_float cymbal::tick() {
    if (counter == 12000) {
        adsr_amp.key_on();
        adsr_cut.key_on();
    } else if (counter == 22000) {
        adsr_amp.key_off();
        adsr_cut.key_off();
    } else if (counter == 24000 - 1) {
        counter = -1;
    }
    ++counter;

    float amp_env = adsr_amp.tick();
    float cut_env = adsr_cut.tick();

    high_pass.m_coeffs = hpf(fs, hpf_f0 + (hpf_f1 * cut_env), hpf_Q);

    float harmonics = 0;
    for (auto i : std::views::iota(0, num_oscs))
        harmonics += osc[i].tick();

    harmonics *= 0.125f;

    return band_pass.tick(high_pass.tick(harmonics * amp_env));
};
