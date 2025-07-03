// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "clover/math.hpp"

#include "lib/mixer_legacy/mixer_000.hpp"

#include "graph.hpp"
#include "patches/patches.hpp"

signal_graph::signal_graph(float fs) : fs{fs} {
    for (auto& t : patch.mix.mixer_tracks)
        mixer_tracks.emplace_back(t);

    audio_mixer = build_mixer_map(mixer_tracks);
}

std::pair<float, float> signal_graph::tick() {
    float out_L = 0;
    float out_R = 0;

    // ----------------
    // KICK
    //
    //

    float kick_dry  = kick.tick();
    float kick_send = kick_dry * audio_mixer.at("kick send");
    kick_dry *= audio_mixer.at("kick dry");
    auto [kick_send_eq, _] = kick_preverb_peq.tick(kick_send, kick_send);

    float kick_wet = kick_verb.tick(kick_send_eq) * audio_mixer.at("kick wet");
    float kick_sum = (kick_dry + kick_wet) * audio_mixer.at("kick bus");

    auto [kick_post_eq, _] = kick_out_peq.tick(kick_sum, kick_sum);
    kick_sum               = kick_post_eq;

    // ----------------
    // CYMBALS
    //
    //

    float ride_cymbal     = ride.tick() * audio_mixer.at("ride") * 0.05f;
    auto [ride_L, ride_R] = ride_peq.tick(ride_cymbal);

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
    float chord_send_L                      = chord_L * audio_mixer.at("chord send");
    float chord_send_R                      = chord_R * audio_mixer.at("chord send");
    auto [chord_preverb_L, chord_preverb_R] = chord_preverb_peq.tick(chord_send_L, chord_send_R);
    chord_preverb_L                         = std::clamp(chord_preverb_L, -1.f, 1.f);
    chord_preverb_R                         = std::clamp(chord_preverb_R, -1.f, 1.f);

    float chord_verb_out_L = chord_verb_L.tick(chord_preverb_L) * audio_mixer.at("chord wet");
    float chord_verb_out_R = chord_verb_R.tick(chord_preverb_R) * audio_mixer.at("chord wet");

    chord_verb_out_L = std::clamp(chord_verb_out_L, -1.f, 1.f);
    chord_verb_out_R = std::clamp(chord_verb_out_R, -1.f, 1.f);

    chord_L *= audio_mixer.at("chord dry");
    chord_R *= audio_mixer.at("chord dry");

    float chord_post_eq_in_L = std::clamp((chord_L + chord_verb_out_L), -1.f, 1.f);
    float chord_post_eq_in_R = std::clamp((chord_R + chord_verb_out_R), -1.f, 1.f);

    auto [chord_post_eq_L, chord_post_eq_R] = chord_peq.tick(chord_post_eq_in_L, chord_post_eq_in_R);

    float chord_sum_L = chord_post_eq_L *= audio_mixer.at("chord bus");
    float chord_sum_R = chord_post_eq_R *= audio_mixer.at("chord bus");

    // ----------------
    // SUMMING
    //
    //

    out_L = kick_sum + ride_L + chord_sum_L;
    out_R = kick_sum + ride_R + chord_sum_R;

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