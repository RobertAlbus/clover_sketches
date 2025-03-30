// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "composition.hpp"

patch_drums_t composition::patch_drums{};
patch_synth_t composition::patch_synth{};
patch_mix_t composition::mix{mix_patch};

std::pair<float, float> composition::tick() {
    float out_L = 0;
    float out_R = 0;

    // ----------------
    // FOUNDATION
    //
    //

    float kick_dry         = kick.drum.tick();
    float kick_send        = kick_dry * mix.kick_send;
    auto [kick_send_eq, _] = kick.preverb_peq.tick(kick_send, kick_send);
    float kick_wet         = kick.verb.tick(kick_send_eq) * mix.kick_wet;
    float kick_sum         = (kick_dry + kick_wet) * mix.kick_gain;

    auto bass_dry               = bass.synth.tick();
    auto [bass_eq_L, bass_eq_R] = bass.out_peq.tick(bass_dry);
    bass_eq_L *= mix.bass_gain;
    bass_eq_R *= mix.bass_gain;

    // ----------------
    // CYMBALS
    //
    //

    float hh1_dry               = cymbals.hh1.tick() * mix.hh1;
    float hh2_dry               = cymbals.hh2.tick() * mix.hh2;
    auto [hh3_dry_L, hh3_dry_R] = cymbals.hh3.tick();
    hh3_dry_L *= mix.hh3;
    hh3_dry_R *= mix.hh3;

    float hh_dry_sum_L = hh1_dry + hh2_dry + hh3_dry_L;
    float hh_dry_sum_R = hh1_dry + hh2_dry + hh3_dry_R;

    float hh_send_L = hh_dry_sum_L * mix.hh_verb_send;
    float hh_send_R = hh_dry_sum_R * mix.hh_verb_send;

    auto [hh_preverb_L, hh_preverb_R] = cymbals.hh_preverb_peq.tick(hh_send_L, hh_send_R);
    float hh_wet_L                    = cymbals.hh_verb_L.tick(hh_preverb_L);
    float hh_wet_R                    = cymbals.hh_verb_R.tick(hh_preverb_R);
    hh_wet_L *= mix.hh_verb_wet;
    hh_wet_R *= mix.hh_verb_wet;

    auto [hh_sum_L, hh_sum_R] = cymbals.hh_peq.tick(hh_wet_L + hh_dry_sum_L, hh_wet_R + hh_dry_sum_R);

    hh_sum_L = std::tanh(hh_sum_L * mix.hh_drive) * mix.hh_trim;
    hh_sum_R = std::tanh(hh_sum_R * mix.hh_drive) * mix.hh_trim;

    hh_sum_L *= mix.hh_bus;
    hh_sum_R *= mix.hh_bus;

    float ride_dry     = cymbals.ride.tick();
    auto [ride_peq, _] = cymbals.ride_peq.tick(ride_dry);
    float ride         = ride_peq * mix.ride;

    float cymbal_bus_L = ride + hh_sum_L;
    float cymbal_bus_R = ride + hh_sum_R;

    // ----------------
    // CHORD
    //
    //

    float chord_L = 0;
    float chord_R = 0;
    for (auto& chord_voice : synth.chord) {
        auto chord_voice_signal = chord_voice.tick();
        chord_L += chord_voice_signal.first;
        chord_R += chord_voice_signal.second;
    }
    float chord_send_L                      = chord_L * mix.chord_send;
    float chord_send_R                      = chord_R * mix.chord_send;
    auto [chord_preverb_L, chord_preverb_R] = synth.chord_peq.tick(chord_send_L, chord_send_R);
    float chord_verb_L                      = synth.chord_verb_L.tick(chord_preverb_L) * mix.chord_wet;
    float chord_verb_R                      = synth.chord_verb_R.tick(chord_preverb_R) * mix.chord_wet;
    auto [chord_post_eq_L, chord_post_eq_R] =
            synth.chord_peq.tick(chord_L + chord_verb_L, chord_R + chord_verb_R);
    float chord_sum_L = chord_post_eq_L * mix.chord_sum;
    float chord_sum_R = chord_post_eq_R * mix.chord_sum;

    // ----------------
    // LEAD
    //
    //

    float lead_a1_L = 0;
    float lead_a1_R = 0;

    auto lead_a1_dry = synth.lead_a[0].tick();
    lead_a1_L += lead_a1_dry.first;
    lead_a1_R += lead_a1_dry.second;
    lead_a1_dry = synth.lead_a[1].tick();
    lead_a1_L += lead_a1_dry.first;
    lead_a1_R += lead_a1_dry.second;

    lead_a1_L *= mix.lead_a;
    lead_a1_R *= mix.lead_a;

    float lead_sum_L = lead_a1_L;
    float lead_sum_R = lead_a1_R;

    auto lead_peq = synth.lead_peq.tick(lead_sum_L, lead_sum_R);

    float lead_mixed_L = lead_peq.first * mix.lead;
    float lead_mixed_R = lead_peq.second * mix.lead;

    // ----------------
    // SUMMING
    //
    //

    out_L = kick_sum + bass_eq_L + cymbal_bus_L + chord_sum_L + lead_mixed_L;
    out_R = kick_sum + bass_eq_R + cymbal_bus_R + chord_sum_R + lead_mixed_R;

    out_L *= gain_master;
    out_R *= gain_master;

    return {out_L, out_R};
}