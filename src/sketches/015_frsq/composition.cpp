// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <print>

#include "composition.hpp"

using namespace clover;
using namespace io;

std::pair<clover_float, clover_float> composition::tick() {
    stsqs.tick();

    float out_L = 0;
    float out_R = 0;

    float chords_L = 0;
    float chords_R = 0;
    for (auto [voice, filter] : std::views::zip(chords, chord_filters)) {
        auto voice_signal = filter.tick(voice.tick());
        chords_L += voice_signal.first;
        chords_R += voice_signal.second;
    }
    chords_L *= chords_mix;
    chords_R *= chords_mix;

    float signal_kick = kick.tick();
    signal_kick *= 1.5f;
    signal_kick = std::clamp(signal_kick, -0.98f, 0.98f);
    signal_kick *= 0.6f;

    float loop_L = signal_kick;
    float loop_R = signal_kick;

    float loop_output_L = loop_L * loop_mix;
    float loop_output_R = loop_R * loop_mix;

    float verb_output_L = fdn_L.tick(loop_L * verb_in_gain) * reverb_mix;
    float verb_output_R = fdn_R.tick(loop_R * verb_in_gain) * reverb_mix;

    out_L = chords_L + loop_output_L + verb_output_L;
    out_R = chords_R + loop_output_R + verb_output_R;

    return {out_L, out_R};
}