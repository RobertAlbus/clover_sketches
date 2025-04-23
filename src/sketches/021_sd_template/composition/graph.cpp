// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "clover/math.hpp"
#include "composition/mix.hpp"
#include "graph.hpp"
#include "patches/patches.hpp"

graph::graph(bar_grid& grid) : grid{grid} {
    for (auto& t : patch::mix.mixer_tracks)
        mixer_tracks.emplace_back(t);
    audio_mixer = build_audio_mixer(mixer_tracks);
}

std::pair<float, float> graph::tick() {
    float out_L = 0;
    float out_R = 0;

    static float& MIX_kick_dry  = audio_mixer.at("kick dry");
    static float& MIX_kick_send = audio_mixer.at("kick send");
    static float& MIX_kick_wet  = audio_mixer.at("kick wet");
    static float& MIX_kick_bus  = audio_mixer.at("kick bus");

    static float& MIX_chord_dry  = audio_mixer.at("chord dry");
    static float& MIX_chord_send = audio_mixer.at("chord send");
    static float& MIX_chord_wet  = audio_mixer.at("chord wet");
    static float& MIX_chord_bus  = audio_mixer.at("chord bus");

    // ----------------
    // FOUNDATION
    //
    //

    float kick_dry  = kick.tick();
    float kick_send = kick_dry * MIX_kick_send;
    kick_dry *= MIX_kick_dry;
    auto [kick_send_eq, _] = kick_preverb_peq.tick(kick_send, kick_send);

    float kick_wet = kick_verb.tick(kick_send_eq) * MIX_kick_wet;  // * kick_auto_verb_send.tick();
    float kick_sum = (kick_dry + kick_wet) * MIX_kick_bus;

    // kick_hpf.m_coeffs = hpf(grid.fs, frequency_by_octave_difference(10, kick_auto_hp.tick()), 0.707);
    // kick_hpf.m_coeffs = hpf(grid.fs, frequency_by_octave_difference(10, 0), 0.707);
    kick_sum = kick_sum;  // kick_hpf.tick(kick_sum);

    auto [kick_post_eq, _] = kick_out_peq.tick(kick_sum, kick_sum);
    kick_sum               = kick_post_eq;

    // ----------------
    // CHORD
    //
    //

    float chord_L = 0;
    float chord_R = 0;
    for (auto& chord_voice : chord) {
        auto chord_voice_signal = chord_voice.tick();
        chord_L += chord_voice_signal.first;
        chord_R += chord_voice_signal.second;
    }
    float chord_send_L                      = chord_L * MIX_chord_send;
    float chord_send_R                      = chord_R * MIX_chord_send;
    auto [chord_preverb_L, chord_preverb_R] = chord_preverb_peq.tick(chord_send_L, chord_send_R);
    chord_preverb_L                         = std::clamp(chord_preverb_L, -1.f, 1.f);
    chord_preverb_R                         = std::clamp(chord_preverb_R, -1.f, 1.f);

    float chord_verb_out_L = chord_verb_L.tick(chord_preverb_L) * MIX_chord_wet;
    float chord_verb_out_R = chord_verb_R.tick(chord_preverb_R) * MIX_chord_wet;

    chord_verb_out_L = std::clamp(chord_verb_out_L, -1.f, 1.f);
    chord_verb_out_R = std::clamp(chord_verb_out_R, -1.f, 1.f);

    chord_L *= MIX_chord_dry;
    chord_R *= MIX_chord_dry;

    float chord_post_eq_in_L = std::clamp((chord_L + chord_verb_out_L), -1.f, 1.f);
    float chord_post_eq_in_R = std::clamp((chord_R + chord_verb_out_R), -1.f, 1.f);

    auto [chord_post_eq_L, chord_post_eq_R] = chord_peq.tick(chord_post_eq_in_L, chord_post_eq_in_R);

    float chord_sum_L = chord_post_eq_L * MIX_chord_bus;
    float chord_sum_R = chord_post_eq_R * MIX_chord_bus;

    // ----------------
    // SUMMING
    //
    //

    out_L = kick_sum + chord_sum_L;
    out_R = kick_sum + chord_sum_R;

    auto main_eq_out = main_eq.tick(out_L, out_R);
    out_L            = main_eq_out.first;
    out_R            = main_eq_out.second;

    out_L *= gain_master;
    out_R *= gain_master;

    out_L = std::tanh(out_L * 1.3f);
    out_R = std::tanh(out_R * 1.3f);

    out_L *= 1.1f;
    out_R *= 1.1f;

    out_L = std::clamp(out_L, -1.f, 1.f);
    out_R = std::clamp(out_R, -1.f, 1.f);

    out_L *= db_to_linear(-0.3f);
    out_R *= db_to_linear(-0.3f);

    return {out_L, out_R};
}