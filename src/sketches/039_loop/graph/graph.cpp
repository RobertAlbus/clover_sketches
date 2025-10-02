// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"

#include "lib/audio_frame/audio_frame.hpp"
#include "lib/audio_frame/audio_frame_math.hpp"
#include "lib/fm/fm_037_update.hpp"
#include "lib/sq/bar_grid_029.hpp"
#include "lib/subtractive_synth/subtractive_synth_038.hpp"
#include "lib/subtractive_synth/subtractive_synth_038_update.hpp"

#include "graph.hpp"

void signal_graph::on_play(double start_bar) {
    kick_hp_env.key_on_from(start_bar);
}

void signal_graph::on_stop() {
    kick_hp_env.key_off();
}

signal_graph::signal_graph(float fs, float bpm, bar_grid_029& grid) : fs{fs}, bpm{bpm}, patch{fs, bpm, grid} {
    snare_body_impulse.oscs[1].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[2].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[3].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[4].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[5].waveform = clover::dsp::wave_noise;

    hh3.oscs[0].waveform = clover::dsp::wave_noise;
}

std::pair<float, float> signal_graph::tick() {
    using float_s = audio_frame;
    float_s out;

    // ----------------
    // SIDECHAIN
    //
    //

    sc_pump.tick();
    const float pump_fast = sc_pump.xs[0] * sc_pump.xs[0];
    const float pump_slow = sc_pump.xs[1] * sc_pump.xs[1];

    const float pump_slow_30 = (pump_slow * 0.3f) + 0.7f;

    const float kick_duck_fast = sc_pump.xs_kick[0] * sc_pump.xs_kick[0];
    const float kick_duck_slow = sc_pump.xs_kick[1] * sc_pump.xs_kick[1];

    const float kick_duck_fast_60 = ((kick_duck_fast * 0.6f) + 0.4f);

    // ----------------
    // AUTOMATION
    //
    //

    float kick_hpf_freq = kick_hp_env.tick();
    kick_hpf.m_coeffs   = clover::dsp::hpf(fs, kick_hpf_freq, 0.707);

    // ----------------
    // KICK
    //
    //

    float_s kick_dry  = kick.tick();
    float_s kick_send = audio_mixer.at("kick send").tick(kick_dry);
    kick_dry          = audio_mixer.at("kick dry").tick(kick_dry);

    float_s kick_send_eq = kick_preverb_peq.tick(kick_send.to_pair());

    float_s kick_wet = kick_verb.tick(kick_send_eq.mid());
    kick_wet *= kick_duck_fast_60;
    kick_wet = audio_mixer.at("kick wet").tick(kick_wet);

    kick_wet = kick_postverb_peq.tick(kick_wet.to_pair());

    float_s kick_sum = kick_dry + kick_wet;
    kick_sum         = kick_out_peq.tick(kick_sum.to_pair());
    kick_sum         = audio_mixer.at("kick bus").tick(kick_sum);

    kick_sum = kick_hpf.tick(kick_sum.L, kick_sum.R);

    // ----------------
    // SNARE
    //
    //

    float_s snare_body_impulse_signal = snare_body_impulse.tick();
    float_s snare_body_impulse_send   = audio_mixer.at("snare impulse send").tick(snare_body_impulse_signal);

    snare_body_impulse_send = clamp(snare_body_impulse_send, -0.999, 0.999);
    float_s snare_body      = snare_body_resonator.tick(snare_body_impulse_send.to_pair());
    snare_body              = clamp(snare_body, -0.999, 0.999);

    // use post-drive snare body for mixing
    float_s snare_body_drive = snare_body_driver.tick(snare_body);
    snare_body_drive         = clamp(snare_body_drive, -0.999, 0.999);

    snare_body_drive = audio_mixer.at("snare body").tick(snare_body_drive);
    snare_body_drive = snare_body_eq.tick(snare_body_drive.to_pair());

    float_s snare = snare_body_drive;
    snare         = audio_mixer.at("snare sum").tick(snare);

    // ----------------
    // CYMBALS
    //
    //

    float_s ride_cymbal = ride.tick();
    ride_cymbal         = audio_mixer.at("ride").tick(ride_cymbal) * 0.05f;
    ride_cymbal         = ride_peq.tick(ride_cymbal.to_pair());

    float_s hh1_cymbal = hh1.tick();
    hh1_cymbal         = audio_mixer.at("hh1").tick(hh1_cymbal) * 0.05f;
    hh1_cymbal         = hh1_driver.tick(hh1_cymbal);
    hh1_cymbal         = hh1_peq.tick(hh1_cymbal.to_pair());

    float_s hh2_cymbal = hh2.tick();
    hh2_cymbal         = audio_mixer.at("hh2").tick(hh2_cymbal) * 0.05f;
    hh2_cymbal         = hh2_driver.tick(hh2_cymbal);
    hh2_cymbal         = hh2_peq.tick(hh2_cymbal.to_pair());

    float_s hh3_cymbal = hh3.tick();
    hh3_cymbal *= pump_slow_30;
    hh3_cymbal = audio_mixer.at("hh3").tick(hh3_cymbal) * 0.05f;
    hh3_cymbal = hh3_peq.tick(hh3_cymbal.to_pair());

    float_s cymbal_sum = hh1_cymbal + hh2_cymbal + hh3_cymbal + ride_cymbal;
    cymbal_sum         = audio_mixer.at("cymbal sum").tick(cymbal_sum);

    // ----------------
    // BASS
    //
    //

    update_fm_037(patch.synth.bass_fm_props, bass_fm);
    float_s bass = bass_fm.tick();
    bass *= kick_duck_fast_60;
    float_s bass_send = bass_preverb_peq.tick(bass.to_pair());
    bass_send         = audio_mixer.at("bass send").tick(bass_send) * 2.f;
    bass              = audio_mixer.at("bass dry").tick(bass) * 0.5f;

    float_s bass_wet = bass_verb.tick(bass_send.to_pair());
    bass_wet         = bass_postverb_peq.tick(bass_wet.to_pair());
    bass_wet         = audio_mixer.at("bass wet").tick(bass_wet);

    float_s bass_out = bass + bass_wet;
    bass_out         = bass_peq.tick(bass_out.to_pair());
    bass_out         = audio_mixer.at("bass bus").tick(bass_out);

    // ----------------
    // CHORD
    //
    //
    update_subtractive_synth(patch.synth.chord_1_props, chord_1);
    float_s chord_1_signal = 0;
    for (auto& chord_voice : chord_1) {
        chord_1_signal += {chord_voice.tick()};
    }
    float_s chord_1_dry = audio_mixer.at("chord 1 dry").tick(chord_1_signal);
    float_s chord_1_verb_signal =
        audio_mixer.at("chord 1 verb").tick(chord_1_verb.tick(chord_1_signal.to_pair()));
    chord_1_dry = chord_1_peq.tick(chord_1_dry.to_pair());

    chord_1_signal = chord_1_dry + chord_1_verb_signal;

    update_subtractive_synth(patch.synth.chord_2_props, chord_2);
    float_s chord_2_signal = 0;
    for (auto& chord_voice : chord_2) {
        chord_2_signal += {chord_voice.tick()};
    }
    float_s chord_2_dry = audio_mixer.at("chord 2 dry").tick(chord_2_signal);
    float_s chord_2_verb_signal =
        audio_mixer.at("chord 2 verb").tick(chord_2_verb.tick((chord_2_signal * 0.5f).to_pair()));
    chord_2_verb_signal *= pump_slow;

    chord_2_dry = chord_2_peq.tick(chord_2_dry.to_pair());

    chord_2_signal = chord_2_dry + chord_2_verb_signal;

    float_s chord_sum = audio_mixer.at("chord bus").tick(chord_1_signal + chord_2_signal);

    // ----------------
    // SUMMING
    //
    //

    out = kick_sum + cymbal_sum + snare + chord_sum + bass_out;

    out = main_eq.tick(out.to_pair());

    out *= gain_master;

    out = tanh(out * 1.3f);

    out = audio_mixer.at("main").tick(out);

    // force -0.3 db
    out = clamp(out, -1.f, 1.f);
    out *= db_to_linear(-0.3f);

    return out.to_pair();
}
