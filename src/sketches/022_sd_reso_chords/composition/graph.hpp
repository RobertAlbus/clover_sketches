#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/filter.hpp"
#include "infrastructure/bar_grid.hpp"

#include "instruments/022_echo.hpp"
#include "instruments/env_bp.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "lib/fdn/fdn.hpp"
#include "lib/kick_drum/kick_drum.hpp"

#include "composition/mix.hpp"
#include "patches/patches.hpp"

struct signal_graph {
    bar_grid& grid;

    std::vector<mixer_track> mixer_tracks;
    std::unordered_map<std::string, std::reference_wrapper<float>> audio_mixer;

    std::pair<float, float> tick();

    float gain_master = 0.5f;
    signal_graph(bar_grid& grid);

    // HACK: semantically meaning label used as a flag for fdn
    // - FDN needs to be updated after props change.
    // - see TODO in FDN
    static constexpr bool COMPONENT_HAS_GUI = true;

    // --------------------------------
    // KICK
    kick_drum_000 kick{grid.fs, patch::drums.kick_drum_props};
    peq kick_preverb_peq{grid.fs, patch::drums.kick_preverb_peq_props};
    fdn8_000 kick_verb{grid.fs, patch::drums.kick_fdn_props, COMPONENT_HAS_GUI};
    peq kick_out_peq{grid.fs, patch::drums.kick_peq_props};

    env_bp kick_auto_hp;
    env_bp kick_auto_verb_send;
    filter kick_hpf{};

    // --------------------------------
    // CHORD

    std::array<subtractive_synth, 6> chord{
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props},
            subtractive_synth{grid.fs, patch::synth.chord_props}};

    peq chord_preverb_peq{grid.fs, patch::synth.chord_preverb_peq_props};
    fdn8_000 chord_verb_L{grid.fs, patch::synth.chord_fdn_props, COMPONENT_HAS_GUI};
    fdn8_000 chord_verb_R{
            grid.fs, patch::synth.chord_fdn_props.taps_mult(1.05f).taps_add(-22.f), COMPONENT_HAS_GUI};
    peq chord_peq{grid.fs, patch::synth.chord_peq_props};

    echoverb_022 chord_echo{
            grid.fs,
            float(grid.bars_to_samples(2)),
            patch::synth.echoverb_props,
            patch::synth.chord_echoverb_peq_props};

    fdn8_000 chord_echo_fb_verb_L{grid.fs, patch::synth.chord_echoverb_fdn_L_props};
    fdn8_000 chord_echo_fb_verb_R{grid.fs, patch::synth.chord_echoverb_fdn_R_props};

    // --------------------------------
    // MAIN BUS

    peq main_eq{grid.fs, patch::mix.main_peq_props};
};
