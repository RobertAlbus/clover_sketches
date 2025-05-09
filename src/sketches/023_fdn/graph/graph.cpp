// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "clover/math.hpp"

#include "lib/mixer/mixer.hpp"

#include "graph.hpp"
#include "patches/patches.hpp"

signal_graph::signal_graph(bar_grid& grid) : grid{grid} {
    for (auto& t : patch::mix.mixer_tracks)
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
    // SUMMING
    //
    //

    out_L = kick_sum;
    out_R = kick_sum;

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