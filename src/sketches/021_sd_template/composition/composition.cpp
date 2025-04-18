// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "clover/math.hpp"
#include "composition.hpp"

automation_patterns composition::automation{};

composition::composition() : counter(sp_bar * duration_bars, duration_bars, should_loop) {
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
    counter.tick();

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
    // SUMMING
    //
    //

    out_L = kick_sum + chord_sum_L;
    out_R = kick_sum + chord_sum_R;

    auto main_eq = main_bus.eq.tick(out_L, out_R);
    out_L        = main_eq.first;
    out_R        = main_eq.second;

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