// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"

#include "lib/audio_frame/audio_frame.hpp"
#include "lib/audio_frame/audio_frame_math.hpp"
#include "lib/fm/fm_037_update.hpp"
#include "lib/subtractive_synth/subtractive_synth_036.hpp"
#include "lib/subtractive_synth/subtractive_synth_036_update.hpp"

#include "graph.hpp"

signal_graph::signal_graph(float fs) : fs{fs} {
    snare_body_impulse.oscs[1].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[2].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[3].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[4].waveform = clover::dsp::wave_noise;
    snare_body_impulse.oscs[5].waveform = clover::dsp::wave_noise;
}

std::pair<float, float> signal_graph::tick() {
    using float_s = audio_frame;
    float_s out;

    // ----------------
    // KICK
    //
    //

    float_s kick_dry  = kick.tick();
    float_s kick_send = audio_mixer.at("kick send").tick(kick_dry);
    kick_dry          = audio_mixer.at("kick dry").tick(kick_dry);

    float_s kick_send_eq = kick_preverb_peq.tick(kick_send.to_pair());

    float_s kick_wet = kick_verb.tick(kick_send_eq.mid());
    kick_wet         = audio_mixer.at("kick wet").tick(kick_wet);

    float_s kick_sum = kick_dry + kick_wet;
    kick_sum         = kick_out_peq.tick(kick_sum.to_pair());
    kick_sum         = audio_mixer.at("kick bus").tick(kick_sum);

    // ----------------
    // SNARE
    //
    //

    float_s snare_body_impulse_signal = snare_body_impulse.tick();
    float_s snare_body_impulse_send   = audio_mixer.at("snare impulse send").tick(snare_body_impulse_signal);

    float_s snare_body = snare_body_resonator.tick(snare_body_impulse_send.to_pair());

    // use post-drive snare body for mixing
    float_s snare_body_drive = snare_body_driver.tick(snare_body);
    snare_body_drive         = audio_mixer.at("snare body").tick(snare_body_drive);
    snare_body_drive         = snare_body_eq.tick(snare_body_drive.to_pair());

    update_subtractive_synth(patch.drums.snare_noise_props, snare_noise);
    float_s snare_noise_signal = snare_noise.tick();
    snare_noise_signal         = snare_noise_eq.tick(snare_noise_signal.to_pair());
    snare_noise_signal         = audio_mixer.at("snare noise").tick(snare_noise_signal);

    float_s snare = snare_body_drive + snare_noise_signal;
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

    float_s cymbal_sum = hh1_cymbal + hh2_cymbal + ride_cymbal;
    cymbal_sum         = audio_mixer.at("cymbal sum").tick(cymbal_sum);

    // ----------------
    // BASS
    //
    //

    update_fm_037(patch.synth.bass_fm_props, bass_fm);
    audio_frame bass = bass_fm.tick();
    float_s bass_out = audio_mixer.at("bass").tick(bass);

    // ----------------
    // CHORD
    //
    //
    update_subtractive_synth(patch.synth.chord_props, chord);

    float_s chord_signal = 0;
    for (auto& chord_voice : chord) {
        chord_signal += {chord_voice.tick()};
    }

    float_s chord_dry = audio_mixer.at("chord dry").tick(chord_signal);
    chord_dry         = chord_preverb_peq.tick(chord_dry.to_pair());

    float_s chord_sum = audio_mixer.at("chord bus").tick(chord_dry);

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
