// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/subtractive_synth/nx_osc_034.hpp"
#include "lib/subtractive_synth/nx_osc_props_smoother_034.hpp"

nx_osc_props_env_034 build_from(const nx_osc_props_034& from) {
    nx_osc_props_env_034 env;

    env.tuning.set(from.tuning, 0);
    env.portamento_time.set(from.portamento_time, 0);
    env.pitch_env_octaves.set(from.pitch_env_octaves, 0);

    size_t size = from.osc_tunings.size();
    for (auto i : std::views::iota(0u, size)) {
        env.osc_tunings.emplace_back();
        env.osc_pans.emplace_back();
        env.osc_gains.emplace_back();
        env.osc_tunings[i].set(from.osc_tunings[i], 0);
        env.osc_pans[i].set(from.osc_pans[i], 0);
        env.osc_gains[i].set(from.osc_gains[i], 0);
    }

    env.retrigger = from.retrigger;
    env.waveforms = from.waveforms;

    env.amp_a.set(from.amp_a, 0);
    env.amp_d.set(from.amp_d, 0);
    env.amp_s.set(from.amp_s, 0);
    env.amp_r.set(from.amp_r, 0);
    env.pitch_a.set(from.pitch_a, 0);
    env.pitch_d.set(from.pitch_d, 0);
    env.pitch_s.set(from.pitch_s, 0);
    env.pitch_r.set(from.pitch_r, 0);

    return env;
}