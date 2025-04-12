#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "instruments/cymbal.hpp"
#include "instruments/env_bp.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/nx_osc.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "patches/patch_drums.hpp"
#include "patches/patch_mix.hpp"
#include "patches/patch_synth.hpp"
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
    double duration_bars      = 226;
    int_fast64_t duration     = int_fast64_t(bar * duration_bars) + 1;

    float gain_master = 0.5f;

    composition();

    static patch_drums_t patch_drums;
    static patch_synth_t patch_synth;
    static patch_mix_t mix;
    static automation_patterns automation;

    // HACK: semantically meaning label used as a flag for fdn
    // - FDN needs to be updated after props change.
    // - see TODO in FDN
    static constexpr bool COMPONENT_HAS_GUI = true;

    struct {
        kick_drum drum{fs, patch_drums.kick_drum_props};
        peq preverb_peq{fs, patch_drums.kick_preverb_peq_props};
        fdn_8_019 verb{fs, patch_drums.kick_fdn_props, COMPONENT_HAS_GUI};
        peq out_peq{fs, patch_drums.kick_peq_props};

        env_bp auto_hp;
        env_bp auto_verb_send;

        filter hpf{};

    } kick;

    struct {
        subtractive_synth synth{fs, patch_drums.bass_props};
        peq out_peq{fs, patch_drums.bass_peq_props};
    } bass;

    struct {
        cymbal hh1{fs, patch_drums.hh1_props};
        cymbal hh2{fs, patch_drums.hh2_props};
        subtractive_synth hh3{fs, patch_drums.hh3_props};

        peq hh_preverb_peq{fs, patch_drums.hh_preverb_peq_props};
        fdn_8_019 hh_verb_L{fs, patch_drums.hh_fdn_props, COMPONENT_HAS_GUI};
        fdn_8_019 hh_verb_R{fs, patch_drums.hh_fdn_props, COMPONENT_HAS_GUI};
        peq hh_peq{fs, patch_drums.hh_peq_props};

        cymbal ride{fs, patch_drums.ride_props};
        peq ride_peq{fs, patch_drums.ride_peq_props};

    } cymbals;

    struct {
        // 3 discrete lead instances with 2 voices each
        std::array<subtractive_synth, 6> lead_a{
                subtractive_synth{fs, patch_synth.lead_a_props},
                subtractive_synth{fs, patch_synth.lead_a_props},
                subtractive_synth{fs, patch_synth.lead_a_props},
                subtractive_synth{fs, patch_synth.lead_a_props},
                subtractive_synth{fs, patch_synth.lead_a_props},
                subtractive_synth{fs, patch_synth.lead_a_props}};
        std::array<nx_osc, 6> lead_b{
                nx_osc{fs, patch_synth.lead_b_props},
                nx_osc{fs, patch_synth.lead_b_props},
                nx_osc{fs, patch_synth.lead_b_props},
                nx_osc{fs, patch_synth.lead_b_props},
                nx_osc{fs, patch_synth.lead_b_props},
                nx_osc{fs, patch_synth.lead_b_props}};

        nx_osc lead_b_lfo{fs, patch_synth.lead_b_lfo_props};

        peq lead_peq{fs, patch_synth.lead_peq_props};

        fdn_8_019 lead_verb_L{
                fs, patch_synth.chord_fdn_props.taps_mult(.495f).taps_add(82.f), COMPONENT_HAS_GUI};
        fdn_8_019 lead_verb_R{
                fs, patch_synth.chord_fdn_props.taps_mult(.5f).taps_add(90.4f), COMPONENT_HAS_GUI};

        env_bp autogain_lead_1;
        env_bp autogain_lead_2;

        std::array<subtractive_synth, 6> chord{
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props}};

        peq chord_preverb_peq{fs, patch_synth.chord_preverb_peq_props};
        fdn_8_019 chord_verb_L{fs, patch_synth.chord_fdn_props, COMPONENT_HAS_GUI};
        fdn_8_019 chord_verb_R{
                fs, patch_synth.chord_fdn_props.taps_mult(1.05f).taps_add(-22.f), COMPONENT_HAS_GUI};
        peq chord_peq{fs, patch_synth.chord_peq_props};

        std::array<subtractive_synth, 12> pad{
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props}};

        peq pad_preverb_peq{fs, patch_synth.pad_preverb_peq_props};
        fdn_8_019 pad_verb_L{fs, patch_synth.pad_fdn_props, COMPONENT_HAS_GUI};
        fdn_8_019 pad_verb_R{fs, patch_synth.pad_fdn_props.taps_add(9.345), COMPONENT_HAS_GUI};
        peq pad_peq{fs, patch_synth.pad_peq_props};

    } synth;

    peq master_peq{fs, patch_synth.master_peq_props};
};
