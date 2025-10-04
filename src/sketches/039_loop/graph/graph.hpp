#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include "graph/instrument/ducker.hpp"
#include "lib/cymbal/cymbal_038.hpp"
#include "lib/env_bp/env_bp_040.hpp"
#include "lib/fdn/fdn8_023.hpp"
#include "lib/fdn/fdn8_stereo_030.hpp"
#include "lib/kick_drum/kick_drum_038.hpp"
#include "lib/mixer/mixer_033.hpp"
#include "lib/peq/peq.hpp"

#include "graph/instrument/driver.hpp"

#include "lib/sq/bar_grid_029.hpp"
#include "lib/subtractive_synth/subtractive_synth_038.hpp"
#include "patches/patches.hpp"

struct signal_graph {
    float fs;
    float bpm;
    patch patch;

    mixer_033 audio_mixer{fs, patch.mix.new_mixer_spec};

    std::pair<float, float> tick();

    float gain_master = 0.5f;
    signal_graph(float fs, float bpm, bar_grid_029& grid);

    void on_play(double start_bar);
    void on_stop();

    // --------------------------------
    // AUTOMATION
    env_bp_040 kick_hp_env{patch.automation.hp_env_kick_hp};

    // --------------------------------
    // SIDECHAIN

    ducker sc_pump{patch.automation.sc_pump_props};

    // --------------------------------
    // KICK

    kick_drum_038 kick{fs, patch.drums.kick_drum_props};
    peq_000 kick_preverb_peq{fs, patch.drums.kick_preverb_peq_props};
    fdn8_023 kick_verb{fs, patch.drums.kick_fdn_props};
    peq_000 kick_postverb_peq{fs, patch.drums.kick_postverb_peq_props};
    peq_000 kick_out_peq{fs, patch.drums.kick_peq_props};
    clover::dsp::filter_2 kick_hpf{};

    // --------------------------------
    // SNARE

    cymbal_038 snare_body_impulse{fs, patch.drums.snare_body_impulse_props};
    fdn8_stereo_030 snare_body_resonator{fs, patch.drums.snare_body_resonator_props};
    driver snare_body_driver{patch.drums.snare_body_driver_props};
    peq_000 snare_body_eq{fs, patch.drums.snare_body_peq_props};

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

    peq_000 bass_preverb_peq{fs, patch.synth.bass_preverb_peq_props};
    fdn8_stereo_030 bass_verb{fs, patch.synth.bass_fdn_props};
    peq_000 bass_postverb_peq{fs, patch.synth.bass_postverb_peq_props};
    peq_000 bass_peq{fs, patch.synth.bass_peq_props};

    // --------------------------------
    // CHORD

    std::array<subtractive_synth_038, 6> chord_1{
        subtractive_synth_038{fs, patch.synth.chord_1_props},
        subtractive_synth_038{fs, patch.synth.chord_1_props},
        subtractive_synth_038{fs, patch.synth.chord_1_props},
        subtractive_synth_038{fs, patch.synth.chord_1_props},
        subtractive_synth_038{fs, patch.synth.chord_1_props},
        subtractive_synth_038{fs, patch.synth.chord_1_props}};

    fdn8_stereo_030 chord_1_verb{fs, patch.synth.chord_1_fdn_props};
    peq_000 chord_1_peq{fs, patch.synth.chord_1_peq_props};

    std::array<subtractive_synth_038, 6> chord_2{
        subtractive_synth_038{fs, patch.synth.chord_2_props},
        subtractive_synth_038{fs, patch.synth.chord_2_props},
        subtractive_synth_038{fs, patch.synth.chord_2_props},
        subtractive_synth_038{fs, patch.synth.chord_2_props},
        subtractive_synth_038{fs, patch.synth.chord_2_props},
        subtractive_synth_038{fs, patch.synth.chord_2_props}};

    fdn8_stereo_030 chord_2_verb{fs, patch.synth.chord_2_fdn_props};
    peq_000 chord_2_peq{fs, patch.synth.chord_2_peq_props};

    peq_000 main_eq{fs, patch.mix.main_peq_props};
};
