// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <format>
#include <ranges>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"

#include "cymbal_038.hpp"

using namespace clover;
using namespace dsp;

std::string cymbal_props_038::to_str() {
    return std::format(
        R"({{
            .freqs            = {{ {}, {}, {}, {}, {}, {} }},
            .hpf_f0           = {},
            .hpf_fmod_octaves = {},
            .hpf_Q            = {},
            .bpf_f0           = {},
            .bpf_Q            = {},
            .amp_adsr         = {},
            .cut_adsr         = {},
    }};)",
        freqs[0],
        freqs[1],
        freqs[2],
        freqs[3],
        freqs[4],
        freqs[5],
        hpf_f0,
        hpf_fmod_octaves,
        hpf_Q,
        bpf_f0,
        bpf_Q,
        static_cast<std::string>(amp_adsr),
        static_cast<std::string>(cut_adsr));
}

cymbal_038::cymbal_038(float fs, const cymbal_props_038& new_props)
    : fs(fs),                       //
      oscs{fs, fs, fs, fs, fs, fs}  //
{
    patch(new_props);
}

void cymbal_038::patch(cymbal_props_038 new_props) {
    props = std::move(new_props);
    set_oscs();
    set_adsrs();
    set_filters();
}

void cymbal_038::set_oscs() {
    for (auto [osc, freq] : std::views::zip(oscs, props.freqs)) {
        osc.freq(freq);
        osc.waveform = wave_square;
    }
}

void cymbal_038::set_adsrs() {
    // Use adsr_values structs for ADSR configuration
    adsr_amp.set(props.amp_adsr.a, props.amp_adsr.d, props.amp_adsr.s, props.amp_adsr.r);
    adsr_cut.set(props.cut_adsr.a, props.cut_adsr.d, props.cut_adsr.s, props.cut_adsr.r);
}

void cymbal_038::set_filters() {
    props.hpf_f0 = std::clamp(props.hpf_f0, 20.f, 20000.f);
    props.bpf_f0 = std::clamp(props.bpf_f0, 20.f, 20000.f);

    high_pass.m_coeffs = hpf(fs, props.hpf_f0, props.hpf_Q);
    band_pass.m_coeffs = bpf(fs, props.bpf_f0, props.bpf_Q);
}

void cymbal_038::key_on() {
    adsr_amp.key_on();
    adsr_cut.key_on();
    adsr_amp.m_elapsed_samples = 0;
    adsr_cut.m_elapsed_samples = 0;
}

void cymbal_038::key_off() {
    adsr_amp.key_off();
    adsr_cut.key_off();
}

float cymbal_038::tick() {
    float amp_env = adsr_amp.tick();
    amp_env *= amp_env;
    float cut_env = adsr_cut.tick();

    float hpf_freq = clover::frequency_by_octave_difference(props.hpf_f0, props.hpf_fmod_octaves * cut_env);
    hpf_freq       = std::clamp(hpf_freq, 20.f, 20000.f);
    high_pass.m_coeffs = hpf(fs, hpf_freq, props.hpf_Q);

    float harmonics = 0;
    for (auto& osc : oscs)
        harmonics += osc.tick();

    harmonics /= cymbal_props_038::num_oscs;

    float hp = high_pass.tick(harmonics * amp_env);
    float bp = band_pass.tick(hp);

    return bp;
};
