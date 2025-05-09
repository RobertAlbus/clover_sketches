// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "clover/math.hpp"

#include "composition.hpp"

patch_drums_t composition::patch_drums{};
patch_synth_t composition::patch_synth{};
patch_mix_t composition::mix{mix_patch};

automation_patterns composition::automation{};

composition::composition() {
    synth.autogain_lead_1.set_pattern(automation.bp_env_lead_a);
    synth.autogain_lead_1.duration_abs = double(duration);
    synth.autogain_lead_1.duration_rel = duration_bars;
    synth.autogain_lead_1.key_on();
    synth.autogain_lead_2.set_pattern(automation.bp_env_lead_b);
    synth.autogain_lead_2.duration_abs = double(duration);
    synth.autogain_lead_2.duration_rel = duration_bars;
    synth.autogain_lead_2.key_on();

    kick.auto_hp.set_pattern(automation.bp_env_kick_hp);
    kick.auto_hp.duration_abs = double(duration);
    kick.auto_hp.duration_rel = duration_bars;
    kick.auto_hp.key_on();
    kick.auto_verb_send.set_pattern(automation.bp_env_kick_verb_send);
    kick.auto_verb_send.duration_abs = double(duration);
    kick.auto_verb_send.duration_rel = duration_bars;
    kick.auto_verb_send.key_on();
}

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

    float kick_wet = kick.verb.tick(kick_send_eq) * mix.kick_wet * kick.auto_verb_send.tick();
    float kick_sum = (kick_dry + kick_wet) * mix.kick_gain;

    kick.hpf.m_coeffs = hpf(fs, frequency_by_octave_difference(10, kick.auto_hp.tick()), 0.707);
    kick_sum          = kick.hpf.tick(kick_sum);

    auto [kick_post_eq, _] = kick.out_peq.tick(kick_sum, kick_sum);
    kick_sum               = kick_post_eq;

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
    hh3_dry_L *= mix.hh3 * 0.07f;
    hh3_dry_R *= mix.hh3 * 0.07f;

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
    float ride         = ride_peq * 0.5f * mix.ride;

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
    auto [chord_preverb_L, chord_preverb_R] = synth.chord_preverb_peq.tick(chord_send_L, chord_send_R);
    chord_preverb_L                         = std::clamp(chord_preverb_L, -1.f, 1.f);
    chord_preverb_R                         = std::clamp(chord_preverb_R, -1.f, 1.f);

    float chord_verb_L = 0;
    float chord_verb_R = 0;

    chord_verb_L = synth.chord_verb_L.tick(chord_preverb_L) * mix.chord_wet;
    chord_verb_R = synth.chord_verb_R.tick(chord_preverb_R) * mix.chord_wet;

    chord_verb_L = std::clamp(chord_verb_L, -1.f, 1.f);
    chord_verb_R = std::clamp(chord_verb_R, -1.f, 1.f);

    chord_L *= mix.chord_dry;
    chord_R *= mix.chord_dry;

    float chord_post_eq_in_L = std::clamp((chord_L + chord_verb_L), -1.f, 1.f);
    float chord_post_eq_in_R = std::clamp((chord_R + chord_verb_R), -1.f, 1.f);

    auto [chord_post_eq_L, chord_post_eq_R] = synth.chord_peq.tick(chord_post_eq_in_L, chord_post_eq_in_R);

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
    auto [pad_preverb_L, pad_preverb_R] = synth.pad_preverb_peq.tick(pad_send_L, pad_send_R);
    float pad_verb_L                    = synth.pad_verb_L.tick(pad_preverb_L) * mix.pad_wet;
    float pad_verb_R                    = synth.pad_verb_R.tick(pad_preverb_R) * mix.pad_wet;
    pad_L *= mix.pad_dry;
    pad_R *= mix.pad_dry;
    auto [pad_post_eq_L, pad_post_eq_R] = synth.pad_peq.tick(pad_L + pad_verb_L, pad_R + pad_verb_R);

    // scaling the signal by 0.02 because it's very loud be default.
    // makes it hard to control with the mixer.
    float pad_sum_L = pad_post_eq_L * mix.pad_sum * 0.02f;  //
    float pad_sum_R = pad_post_eq_R * mix.pad_sum * 0.02f;  //

    // ----------------
    // LEAD
    //
    //

    float lead_1_autogain = synth.autogain_lead_1.tick();
    float lead_2_autogain = synth.autogain_lead_2.tick();

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

    lead_a1_L *= lead_1_autogain;
    lead_a1_R *= lead_1_autogain;

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

    lead_a2_L *= lead_2_autogain;
    lead_a2_R *= lead_2_autogain;

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

    lead_b1_L *= lead_1_autogain;
    lead_b1_R *= lead_1_autogain;

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

    lead_b2_L *= lead_2_autogain;
    lead_b2_R *= lead_2_autogain;

    auto [lead_b_lfo, _]   = synth.lead_b_lfo.tick();
    float lead_b_lfo_clamp = std::clamp(((lead_b_lfo + 1.f) * 0.5f), -1.f, 1.f);

    float lead_b_L = (lead_b1_L + lead_b2_L) * lead_b_lfo;
    float lead_b_R = (lead_b1_R + lead_b2_R) * lead_b_lfo;

    // LEAD RINGMOD
    float lead_ringmod_L = 0;  // lead_a_L * lead_b_L;
    float lead_ringmod_R = 0;  // lead_a_R * lead_b_R;

    for (auto& lead_a_voice : synth.lead_a)
        lead_a_voice.osc.input_mod_pitch_octaves = lead_b_L * 1000 * mix.lead_ringmod;

    float lead_sum_L =
            (lead_a_L * mix.lead_a) + (lead_b_L * mix.lead_b) + (lead_ringmod_L * mix.lead_ringmod);
    float lead_sum_R =
            (lead_a_R * mix.lead_a) + (lead_b_R * mix.lead_b) + (lead_ringmod_R * mix.lead_ringmod);

    // TODO

    float lead_verb_L = synth.lead_verb_L.tick(lead_sum_L) * 0.1f;
    float lead_verb_R = synth.lead_verb_R.tick(lead_sum_R) * 0.1f;

    lead_sum_L += lead_verb_L;
    lead_sum_R += lead_verb_R;

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

    // WHY IS DC BLOCKER MAKING THE TOP END MONO?
    // IT'S THE HPF REGARDLESS OF SETTINGS!
    auto out_dc_blocker = master_peq.tick(out_L, out_R);
    out_L               = out_dc_blocker.first;
    out_R               = out_dc_blocker.second;

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
