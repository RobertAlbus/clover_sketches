#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/filter.hpp"

#include "lib/sq/bar_grid.hpp"

#include "lib/echo/echo.hpp"
#include "lib/env_bp/env_bp.hpp"
#include "lib/fdn/fdn8_000.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "lib/mixer_legacy/mixer_000.hpp"
#include "lib/peq/peq.hpp"
#include "lib/subtractive_synth/subtractive_synth.hpp"

#include "patches/patches.hpp"

struct signal_graph {
    bar_grid& grid;

    std::vector<mixer_track_000> mixer_tracks;
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
    peq_000 kick_preverb_peq{grid.fs, patch::drums.kick_preverb_peq_props};
    fdn8_000 kick_verb{grid.fs, patch::drums.kick_fdn_props, COMPONENT_HAS_GUI};
    peq_000 kick_out_peq{grid.fs, patch::drums.kick_peq_props};

    env_bp_000 kick_auto_hp;
    env_bp_000 kick_auto_verb_send;
    filter kick_hpf{};

    // --------------------------------
    // CHORD

    std::array<subtractive_synth_000, 6> chord{
            subtractive_synth_000{grid.fs, patch::synth.chord_props},
            subtractive_synth_000{grid.fs, patch::synth.chord_props},
            subtractive_synth_000{grid.fs, patch::synth.chord_props},
            subtractive_synth_000{grid.fs, patch::synth.chord_props},
            subtractive_synth_000{grid.fs, patch::synth.chord_props},
            subtractive_synth_000{grid.fs, patch::synth.chord_props}};

    peq_000 chord_preverb_peq{grid.fs, patch::synth.chord_preverb_peq_props};
    fdn8_000 chord_verb_L{grid.fs, patch::synth.chord_fdn_props_L, COMPONENT_HAS_GUI};
    fdn8_000 chord_verb_R{grid.fs, patch::synth.chord_fdn_props_R, COMPONENT_HAS_GUI};
    peq_000 chord_peq{grid.fs, patch::synth.chord_peq_props};

    echo_000 chord_echo{
            grid.fs,
            float(grid.bars_to_samples(2)),
            patch::synth.echoverb_props,
            patch::synth.chord_echoverb_peq_props};

    fdn8_000 chord_echo_fb_verb_L{grid.fs, patch::synth.chord_echoverb_fdn_L_props};
    fdn8_000 chord_echo_fb_verb_R{grid.fs, patch::synth.chord_echoverb_fdn_R_props};

    // --------------------------------
    // MAIN BUS

    peq_000 main_eq{grid.fs, patch::mix.main_peq_props};
};
