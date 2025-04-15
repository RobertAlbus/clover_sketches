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
    update_peq_from_gui(kick_peq_gui, kick_peq);

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

    float drones_L = 0;
    float drones_R = 0;
    for (auto& drone : drones) {
        auto voice_signal = drone.tick();
        drones_L += voice_signal.first;
        drones_R += voice_signal.second;
    }
    drones_L *= mix_beep;
    drones_R *= mix_beep;

    float signal_kick = kick.tick();
    signal_kick *= 1.5f;
    signal_kick = std::clamp(signal_kick, -0.98f, 0.98f);
    signal_kick *= 0.6f;
    signal_kick *= kick_mix;

    float kick_verb_L = kick_fdn_L.tick(signal_kick * verb_in_gain) * reverb_mix;
    float kick_verb_R = kick_fdn_R.tick(signal_kick * verb_in_gain) * reverb_mix;

    float kick_sum_L = signal_kick + kick_verb_L;
    float kick_sum_R = signal_kick + kick_verb_R;

    auto [kick_eqd_L, kick_eqd_R] = kick_peq.tick({kick_sum_L, kick_sum_R});

    float signal_hh1 = hh1.tick() * hh1_mix;
    float signal_hh2 = hh2.tick() * hh2_mix * hh2_articulation.tick();

    float drums_output_L = kick_eqd_L + signal_hh1 + signal_hh2;
    float drums_output_R = kick_eqd_R + signal_hh1 + signal_hh2;

    out_L = chords_L + drums_output_L + drones_L;
    out_R = chords_R + drums_output_R + drones_R;

    out_L = std::clamp(std::tanh(out_L * 2.5f) * 1.4f, -1.f, 1.f);
    out_R = std::clamp(std::tanh(out_R * 2.5f) * 1.4f, -1.f, 1.f);

    return {out_L, out_R};
}