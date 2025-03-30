// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "composition.hpp"
#include "patches/patch_synth.hpp"

patch_drums_t composition::patch_drums{};
patch_synth_t composition::patch_synth{};

std::pair<float, float> composition::tick() {
    float out_L = 0;
    float out_R = 0;

    float kick_dry         = kick.drum.tick();
    float kick_send        = kick_dry * patch_drums.kick_mix.send;
    auto [kick_send_eq, _] = kick.preverb_peq.tick(kick_send, kick_send);
    float kick_wet         = kick.verb.tick(kick_send_eq) * patch_drums.kick_mix.wet;
    float kick_sum         = (kick_dry + kick_wet) * patch_drums.kick_mix.gain;

    auto bass_dry               = bass.synth.tick();
    auto [bass_eq_L, bass_eq_R] = bass.out_peq.tick(bass_dry);
    bass_eq_L *= patch_drums.bass_mix.gain;
    bass_eq_R *= patch_drums.bass_mix.gain;

    float hh1_dry               = cymbals.hh1.tick() * patch_drums.hh_mix.mix_hh1;
    float hh2_dry               = cymbals.hh2.tick() * patch_drums.hh_mix.mix_hh2;
    auto [hh3_dry_L, hh3_dry_R] = cymbals.hh3.tick();
    hh3_dry_L *= patch_drums.hh_mix.mix_hh3;
    hh3_dry_R *= patch_drums.hh_mix.mix_hh3;

    float hh_dry_sum_L = hh1_dry + hh2_dry + hh3_dry_L;
    float hh_dry_sum_R = hh1_dry + hh2_dry + hh3_dry_R;

    float hh_send_L = hh_dry_sum_L * patch_drums.hh_mix.hh_verb_send;
    float hh_send_R = hh_dry_sum_R * patch_drums.hh_mix.hh_verb_send;

    auto [hh_preverb_L, hh_preverb_R] = cymbals.hh_preverb_peq.tick(hh_send_L, hh_send_R);
    float hh_wet_L                    = cymbals.hh_verb_L.tick(hh_preverb_L);
    float hh_wet_R                    = cymbals.hh_verb_R.tick(hh_preverb_R);
    hh_wet_L *= patch_drums.hh_mix.hh_verb_wet;
    hh_wet_R *= patch_drums.hh_mix.hh_verb_wet;

    auto [hh_sum_L, hh_sum_R] = cymbals.hh_peq.tick(hh_wet_L + hh_dry_sum_L, hh_wet_R + hh_dry_sum_R);

    hh_sum_L = std::tanh(hh_sum_L * patch_drums.hh_mix.mix_drive) * patch_drums.hh_mix.mix_trim;
    hh_sum_R = std::tanh(hh_sum_R * patch_drums.hh_mix.mix_drive) * patch_drums.hh_mix.mix_trim;

    hh_sum_L *= patch_drums.hh_mix.mix_hh;
    hh_sum_R *= patch_drums.hh_mix.mix_hh;

    float chord_L = 0;
    float chord_R = 0;
    for (auto& chord_voice : synth.chord) {
        auto chord_voice_signal = chord_voice.tick();
        chord_L += chord_voice_signal.first;
        chord_R += chord_voice_signal.second;
    }
    float chord_send_L                      = chord_L * patch_synth.chord_mix.send;
    float chord_send_R                      = chord_R * patch_synth.chord_mix.send;
    auto [chord_preverb_L, chord_preverb_R] = synth.chord_peq.tick(chord_send_L, chord_send_R);
    float chord_verb_L = synth.chord_verb_L.tick(chord_preverb_L) * patch_synth.chord_mix.wet;
    float chord_verb_R = synth.chord_verb_R.tick(chord_preverb_R) * patch_synth.chord_mix.wet;
    auto [chord_post_eq_L, chord_post_eq_R] =
            synth.chord_peq.tick(chord_L + chord_verb_L, chord_R + chord_verb_R);
    float chord_sum_L = chord_post_eq_L * patch_synth.chord_mix.sum;
    float chord_sum_R = chord_post_eq_R * patch_synth.chord_mix.sum;

    // chord_peq

    out_L = kick_sum + bass_eq_L + hh_sum_L + chord_sum_L;
    out_R = kick_sum + bass_eq_R + hh_sum_R + chord_sum_R;

    out_L *= gain_master;
    out_R *= gain_master;

    return {out_L, out_R};
}