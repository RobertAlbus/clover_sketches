// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/math.hpp"

#include "cymbal_024.hpp"

using namespace clover;
using namespace dsp;

clover_float cymbal_024::tick() {
    float amp_env = adsr_amp.tick();
    float cut_env = adsr_cut.tick();

    float hpf_freq = frequency_by_octave_difference(props.hpf_f0, props.hpf_fmod_octaves * cut_env);

    high_pass.m_coeffs = hpf(fs, hpf_freq, props.hpf_Q);

    float harmonics = 0;
    for (auto& osc : oscs)
        harmonics += osc.tick();

    harmonics /= cymbal_props_000::num_oscs;

    float hp = high_pass.tick(harmonics * amp_env);
    float bp = band_pass.tick(hp);

    return bp;
};
