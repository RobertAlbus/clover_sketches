// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>
#include <ranges>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"

using namespace clover;
using namespace dsp;

struct cymbal {
    float fs = 48000.f;
    int fs_i = int(fs);

    static const int num_oscs = 6;
    oscillator osc[num_oscs]  = {{fs}, {fs}, {fs}, {fs}, {fs}, {fs}};
    float osc_freq[num_oscs]  = {199, 215, 253, 307, 329, 405};

    env_adsr adsr_amp;
    env_adsr adsr_cut;

    filter high_pass;
    filter band_pass;

    float hpf_f0 = 1000;
    float hpf_f1 = 4000;
    float hpf_Q  = 2;
    float bpf_f0 = 4000;
    float bpf_Q  = 1.3;

    float amp_a = 1;
    float amp_d = 48 * 20;
    float amp_s = 0;
    float amp_r = 1;

    float cut_a = 1;
    float cut_d = 48 * 20;
    float cut_s = 0;
    float cut_r = 1;

    cymbal() {
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

    int_fast64_t counter = 0;
    clover_float tick() {
        if (counter == 12000) {
            adsr_amp.key_on();
            adsr_cut.key_on();
        } else if (counter == 14000) {
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
};
