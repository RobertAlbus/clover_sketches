#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "graph/instrument/ducker.hpp"
#include "lib/cymbal/cymbal_038.hpp"
#include "lib/fdn/fdn8_023.hpp"
#include "lib/fdn/fdn8_stereo_030.hpp"
#include "lib/kick_drum/kick_drum_038.hpp"
#include "lib/mixer/mixer_033.hpp"
#include "lib/peq/peq.hpp"

#include "graph/instrument/driver.hpp"

#include "lib/subtractive_synth/subtractive_synth_038.hpp"
#include "patches/patches.hpp"

struct signal_graph {
    float fs;
    float bpm;
    patch patch;

    mixer_033 audio_mixer{fs, patch.mix.new_mixer_spec};

    std::pair<float, float> tick();

    float gain_master = 0.5f;
    signal_graph(float fs, float bpm);

    // --------------------------------
    // SIDECHAIN

    ducker sc_pump{patch.automation.sc_pump_props};

    // --------------------------------
    // KICK

    kick_drum_038 kick{fs, patch.drums.kick_drum_props};
    peq_000 kick_preverb_peq{fs, patch.drums.kick_preverb_peq_props};
    fdn8_023 kick_verb{fs, patch.drums.kick_fdn_props};
    peq_000 kick_out_peq{fs, patch.drums.kick_peq_props};

    // --------------------------------
    // SNARE

    cymbal_038 snare_body_impulse{fs, patch.drums.snare_body_impulse_props};
    fdn8_stereo_030 snare_body_resonator{fs, patch.drums.snare_body_resonator_props};
    driver snare_body_driver{patch.drums.snare_body_driver_props};
    peq_000 snare_body_eq{fs, patch.drums.snare_body_peq_props};

    subtractive_synth_038 snare_noise{fs, patch.drums.snare_noise_props};
    peq_000 snare_noise_eq{fs, patch.drums.snare_noise_peq_props};

    // --------------------------------
    // RIDE

    cymbal_038 ride{fs, patch.drums.ride_props};
    peq_000 ride_peq{fs, patch.drums.ride_peq_props};

    // --------------------------------
    // HH

    cymbal_038 hh1{fs, patch.drums.hh1_props};
    driver hh1_driver{patch.drums.hh1_driver_props};
    peq_000 hh1_peq{fs, patch.drums.hh1_peq_props};

    cymbal_038 hh2{fs, patch.drums.hh2_props};
    driver hh2_driver{patch.drums.hh2_driver_props};
    peq_000 hh2_peq{fs, patch.drums.hh2_peq_props};

    cymbal_038 hh3{fs, patch.drums.hh3_props};
    peq_000 hh3_peq{fs, patch.drums.hh3_peq_props};

    // --------------------------------
    // BASS

    fm_037 bass_fm{fs, patch.synth.bass_fm_props};

    // --------------------------------
    // CHORD

    std::array<subtractive_synth_038, 6> chord{
        subtractive_synth_038{fs, patch.synth.chord_props},
        subtractive_synth_038{fs, patch.synth.chord_props},
        subtractive_synth_038{fs, patch.synth.chord_props},
        subtractive_synth_038{fs, patch.synth.chord_props},
        subtractive_synth_038{fs, patch.synth.chord_props},
        subtractive_synth_038{fs, patch.synth.chord_props}};

    fdn8_stereo_030 chord_verb{fs, patch.synth.chord_fdn_props};
    peq_000 chord_peq{fs, patch.synth.chord_preverb_peq_props};

    peq_000 main_eq{fs, patch.mix.main_peq_props};
};
