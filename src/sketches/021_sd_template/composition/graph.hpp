#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "infrastructure/bar_grid.hpp"

#include "instruments/env_bp.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/automation.hpp"

#include "bar_counter.hpp"
#include "composition/mix.hpp"
#include "patches/patches.hpp"

struct graph {
    bar_grid& grid;

    std::vector<mixer_track> mixer_tracks;
    std::unordered_map<std::string, std::reference_wrapper<float>> audio_mixer;

    std::pair<float, float> tick();

    int channel_count_out = 2;

    float gain_master = 0.5f;

    static automation_patterns automation;
    graph(bar_grid& grid);

    bar_counter counter;

    // HACK: semantically meaning label used as a flag for fdn
    // - FDN needs to be updated after props change.
    // - see TODO in FDN
    static constexpr bool COMPONENT_HAS_GUI = true;

    kick_drum kick{grid.fs, patch::drums.kick_drum_props};
    peq kick_preverb_peq{grid.fs, patch::drums.kick_preverb_peq_props};
    fdn_8_019 kick_verb{grid.fs, patch::drums.kick_fdn_props, COMPONENT_HAS_GUI};
    peq kick_out_peq{grid.fs, patch::drums.kick_peq_props};

    env_bp kick_auto_hp;
    env_bp kick_auto_verb_send;
    filter kick_hpf{};

    std::array<subtractive_synth, 6> chord{
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props}};

    peq chord_preverb_peq{grid.fs, patch::synth.chord_preverb_peq_props};
    fdn_8_019 chord_verb_L{grid.fs, patch::synth.chord_fdn_props, COMPONENT_HAS_GUI};
    fdn_8_019 chord_verb_R{
            grid.fs, patch::synth.chord_fdn_props.taps_mult(1.05f).taps_add(-22.f), COMPONENT_HAS_GUI};
    peq chord_peq{grid.fs, patch::synth.chord_peq_props};

    peq main_eq{grid.fs, patch::mix.main_peq_props};
};
