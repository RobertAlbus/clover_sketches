// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"

#include "cymbal.hpp"

using namespace clover;
using namespace dsp;

cymbal::cymbal(clover_float fs, const cymbal_props& new_props)
    : fs(fs),                       //
      oscs{fs, fs, fs, fs, fs, fs}  //
{
    patch(new_props);
}

void cymbal::patch(cymbal_props new_props) {
    props = std::move(new_props);
    set_oscs();
    set_adsrs();
    set_filters();
}

void cymbal::set_oscs() {
    for (auto [osc, freq] : std::views::zip(oscs, props.freqs)) {
        osc.freq(freq);
        osc.waveform = wave_square;
    }
}

void cymbal::set_adsrs() {
    adsr_amp.set(props.amp_a, props.amp_d, props.amp_s, props.amp_r);
    adsr_cut.set(props.cut_a, props.cut_d, props.cut_s, props.cut_r);
}

void cymbal::set_filters() {
    high_pass.m_coeffs = hpf(fs, props.hpf_f0, props.hpf_Q);
    band_pass.m_coeffs = bpf(fs, props.bpf_f0, props.bpf_Q);
}

void cymbal::key_on() {
    adsr_amp.key_on();
    adsr_cut.key_on();
}

void cymbal::key_off() {
    adsr_amp.key_off();
    adsr_cut.key_off();
}

clover_float cymbal::tick() {
    float amp_env = adsr_amp.tick();
    float cut_env = adsr_cut.tick();

    float hpf_freq = clover::frequency_by_octave_difference(props.hpf_f0, props.hpf_fmod_octaves * cut_env);

    high_pass.m_coeffs = hpf(fs, hpf_freq, props.hpf_Q);

    float harmonics = 0;
    for (auto& osc : oscs)
        harmonics += osc.tick();

    static const float gain = 1.f / std::sqrt(float(cymbal_props::num_oscs));
    harmonics *= gain;

    float hp = high_pass.tick(harmonics * amp_env);
    float bp = band_pass.tick(hp);

    return bp;
};
