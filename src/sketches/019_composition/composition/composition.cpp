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

    float cymbal_bus_L = (ride + hh_sum_L) * mix.cymbal_bus;
    float cymbal_bus_R = (ride + hh_sum_R) * mix.cymbal_bus;

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
    // PAD
    //
    //

    float pad_L = 0;
    float pad_R = 0;
    for (auto& pad_voice : synth.pad) {
        auto pad_voice_signal = pad_voice.tick();
        pad_L += pad_voice_signal.first;
        pad_R += pad_voice_signal.second;
    }
    float pad_send_L                    = pad_L * mix.pad_send;
    float pad_send_R                    = pad_R * mix.pad_send;
    auto [pad_preverb_L, pad_preverb_R] = synth.pad_peq.tick(pad_send_L, pad_send_R);
    float pad_verb_L                    = synth.pad_verb_L.tick(pad_preverb_L) * mix.pad_wet;
    float pad_verb_R                    = synth.pad_verb_R.tick(pad_preverb_R) * mix.pad_wet;
    auto [pad_post_eq_L, pad_post_eq_R] = synth.pad_peq.tick(pad_L + pad_verb_L, pad_R + pad_verb_R);

    // scaling the signal by 0.02 because it's very loud be default.
    // makes it hard to control with the mixer.
    float pad_sum_L = pad_post_eq_L * mix.pad_sum * 0.02f;  //
    float pad_sum_R = pad_post_eq_R * mix.pad_sum * 0.02f;  //

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
    lead_a1_dry = synth.lead_a[2].tick();
    lead_a1_L += lead_a1_dry.first;
    lead_a1_R += lead_a1_dry.second;

    float lead_a2_L = 0;
    float lead_a2_R = 0;

    auto lead_a2_dry = synth.lead_a[3].tick();
    lead_a2_L += lead_a2_dry.first;
    lead_a2_R += lead_a2_dry.second;
    lead_a2_dry = synth.lead_a[4].tick();
    lead_a2_L += lead_a2_dry.first;
    lead_a2_R += lead_a2_dry.second;
    lead_a2_dry = synth.lead_a[5].tick();
    lead_a2_L += lead_a2_dry.first;
    lead_a2_R += lead_a2_dry.second;

    float lead_a_L = lead_a1_L + lead_a2_L;
    float lead_a_R = lead_a1_R + lead_a2_R;

    float lead_b1_L = 0;
    float lead_b1_R = 0;

    auto lead_b1_dry = synth.lead_b[0].tick();
    lead_b1_L += lead_b1_dry.first;
    lead_b1_R += lead_b1_dry.second;
    lead_b1_dry = synth.lead_b[1].tick();
    lead_b1_L += lead_b1_dry.first;
    lead_b1_R += lead_b1_dry.second;
    lead_b1_dry = synth.lead_b[2].tick();
    lead_b1_L += lead_b1_dry.first;
    lead_b1_R += lead_b1_dry.second;

    float lead_b2_L = 0;
    float lead_b2_R = 0;

    auto lead_b2_dry = synth.lead_b[3].tick();
    lead_b2_L += lead_b2_dry.first;
    lead_b2_R += lead_b2_dry.second;
    lead_b2_dry = synth.lead_b[4].tick();
    lead_b2_L += lead_b2_dry.first;
    lead_b2_R += lead_b2_dry.second;
    lead_b2_dry = synth.lead_b[5].tick();
    lead_b2_L += lead_b2_dry.first;
    lead_b2_R += lead_b2_dry.second;

    float lead_b_L = lead_b1_L + lead_b2_L;
    float lead_b_R = lead_b1_R + lead_b2_R;

    // LEAD RINGMOD
    float lead_ringmod_L = lead_a_L * lead_b_L;
    float lead_ringmod_R = lead_a_R * lead_b_R;

    float lead_sum_L =
            (lead_a_L * mix.lead_a) + (lead_b_L * mix.lead_b) + (lead_ringmod_L * mix.lead_ringmod);
    float lead_sum_R =
            (lead_a_R * mix.lead_a) + (lead_b_R * mix.lead_b) + (lead_ringmod_R * mix.lead_ringmod);

    // LEAD SUM
    auto lead_peq      = synth.lead_peq.tick(lead_sum_L, lead_sum_R);
    float lead_mixed_L = lead_peq.first * mix.lead;
    float lead_mixed_R = lead_peq.second * mix.lead;

    // ----------------
    // SUMMING
    //
    //

    out_L = kick_sum + bass_eq_L + cymbal_bus_L + chord_sum_L + lead_mixed_L + pad_sum_L;
    out_R = kick_sum + bass_eq_R + cymbal_bus_R + chord_sum_R + lead_mixed_R + pad_sum_R;

    out_L *= gain_master;
    out_R *= gain_master;

    out_L = std::clamp(out_L, -1.f, 1.f);
    out_R = std::clamp(out_R, -1.f, 1.f);

    out_L *= 0.95f;
    out_R *= 0.95f;

    return {out_L, out_R};
}