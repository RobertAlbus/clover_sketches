#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "composition/mix.hpp"
#include "instruments/env_bp.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"

#include "patches/patches.hpp"

#include "sequence/automation.hpp"

struct composition {
    std::pair<float, float> tick();

    static constexpr float fs = 48000;
    int fs_i                  = static_cast<int>(fs);
    int channel_count_out     = 2;
    double bpm                = 142;
    double spm                = fs * 60;
    double spb                = spm / bpm;
    double bar                = spb * 4;
    double beat               = spb;
    double duration_bars      = 32;
    bool should_loop          = true;
    int_fast64_t duration     = int_fast64_t(bar * duration_bars) + 1;

    float gain_master = 0.5f;

    composition();

    static automation_patterns automation;

    // HACK: semantically meaning label used as a flag for fdn
    // - FDN needs to be updated after props change.
    // - see TODO in FDN
    static constexpr bool COMPONENT_HAS_GUI = true;

    struct {
        kick_drum drum{fs, patch::drums.kick_drum_props};
        peq preverb_peq{fs, patch::drums.kick_preverb_peq_props};
        fdn_8_019 verb{fs, patch::drums.kick_fdn_props, COMPONENT_HAS_GUI};
        peq out_peq{fs, patch::drums.kick_peq_props};

        env_bp auto_hp;
        env_bp auto_verb_send;

        filter hpf{};

    } kick;

    struct {
        std::array<subtractive_synth, 6> chord{
                subtractive_synth{fs, patch::synth.chord_props},
                subtractive_synth{fs, patch::synth.chord_props},
                subtractive_synth{fs, patch::synth.chord_props},
                subtractive_synth{fs, patch::synth.chord_props},
                subtractive_synth{fs, patch::synth.chord_props},
                subtractive_synth{fs, patch::synth.chord_props}};

        peq chord_preverb_peq{fs, patch::synth.chord_preverb_peq_props};
        fdn_8_019 chord_verb_L{fs, patch::synth.chord_fdn_props, COMPONENT_HAS_GUI};
        fdn_8_019 chord_verb_R{
                fs, patch::synth.chord_fdn_props.taps_mult(1.05f).taps_add(-22.f), COMPONENT_HAS_GUI};
        peq chord_peq{fs, patch::synth.chord_peq_props};
    } synth;

    struct {
        peq eq{fs, patch::mix.main_peq_props};
    } main_bus;
    composition_mix mix{patch::mix.mix};
};
