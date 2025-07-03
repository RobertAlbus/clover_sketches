// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"

#include "lib/audio_frame/audio_frame.hpp"
#include "lib/audio_frame/audio_frame_math.hpp"

#include "graph.hpp"

signal_graph::signal_graph(float fs) : fs{fs} {
    snare_impulse.oscs[1].waveform = clover::dsp::wave_noise;
    snare_impulse.oscs[2].waveform = clover::dsp::wave_noise;
    snare_impulse.oscs[3].waveform = clover::dsp::wave_noise;
    snare_impulse.oscs[4].waveform = clover::dsp::wave_noise;
    snare_impulse.oscs[5].waveform = clover::dsp::wave_noise;
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

    float_s snare_impulse_signal = snare_impulse.tick();
    float_s snare_impulse_send   = audio_mixer.at("snare impulse send").tick(snare_impulse_signal);
    snare_impulse_signal         = audio_mixer.at("snare impulse").tick(snare_impulse_signal);

    float_s snare_body = snare_resonator.tick(snare_impulse_send.to_pair());

    float_s snare_tail_send = snare_body + snare_impulse_signal;
    float_s snare_tail      = snare_verb.tick(snare_tail_send);
    snare_tail              = audio_mixer.at("snare verb").tick(snare_tail);

    // use post-drive snare body for mixing
    float_s snare_body_drive = snare_driver.tick(snare_body);
    snare_body_drive         = audio_mixer.at("snare body").tick(snare_body_drive);

    float_s snare = snare_impulse_signal + snare_body_drive + snare_tail;
    snare         = snare_eq.tick(snare.to_pair());
    snare         = audio_mixer.at("snare sum").tick(snare);

    // ----------------
    // CYMBALS
    //
    //

    float_s ride_cymbal = ride.tick();
    ride_cymbal         = audio_mixer.at("ride").tick(ride_cymbal) * 0.05f;
    ride_cymbal         = ride_peq.tick(ride_cymbal.to_pair());

    // ----------------
    // CHORD
    //
    //

    float_s chord_signal = 0;
    for (auto& chord_voice : chord) {
        chord_signal += {chord_voice.tick()};
    }
    float_s chord_send    = audio_mixer.at("chord send").tick(chord_signal);
    float_s chord_preverb = chord_preverb_peq.tick(chord_send.to_pair());
    chord_preverb         = clamp(chord_preverb, -1.f, 1.f);

    float_s chord_verb_out = chord_verb.tick(chord_preverb.to_pair());
    chord_verb_out         = audio_mixer.at("chord wet").tick(chord_verb_out);

    chord_verb_out = clamp(chord_verb_out, -1.f, 1.f);

    chord_signal = audio_mixer.at("chord dry").tick(chord_signal);

    float_s chord_post_eq_in = clamp((chord_signal + chord_verb_out), -1.f, 1.f);
    float_s chord_post_eq    = chord_peq.tick(chord_post_eq_in.to_pair());

    float_s chord_sum = audio_mixer.at("chord bus").tick(chord_post_eq);

    // ----------------
    // SUMMING
    //
    //

    out = kick_sum + ride_cymbal + snare + chord_sum;

    out = main_eq.tick(out.to_pair());

    out *= gain_master;

    out = tanh(out * 1.3f);

    out = audio_mixer.at("main").tick(out);

    // force -0.3 db
    out = clamp(out, -1.f, 1.f);
    out *= db_to_linear(-0.3f);

    return out.to_pair();
}
