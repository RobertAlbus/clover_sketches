#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "lib/cymbal/cymbal_000.hpp"
#include "lib/env_bp/env_bp.hpp"
#include "lib/fdn/fdn8_000.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "lib/subtractive_synth/subtractive_synth.hpp"

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
        kick_drum_000 drum{fs, patch_drums.kick_drum_props};
        peq_000 preverb_peq{fs, patch_drums.kick_preverb_peq_props};
        fdn8_000 verb{fs, patch_drums.kick_fdn_props, COMPONENT_HAS_GUI};
        peq_000 out_peq{fs, patch_drums.kick_peq_props};

        env_bp_000 auto_hp;
        env_bp_000 auto_verb_send;

        filter hpf{};

    } kick;

    struct {
        subtractive_synth_000 synth{fs, patch_drums.bass_props};
        peq_000 out_peq{fs, patch_drums.bass_peq_props};
    } bass;

    struct {
        cymbal_000 hh1{fs, patch_drums.hh1_props};
        cymbal_000 hh2{fs, patch_drums.hh2_props};
        subtractive_synth_000 hh3{fs, patch_drums.hh3_props};

        peq_000 hh_preverb_peq{fs, patch_drums.hh_preverb_peq_props};
        fdn8_000 hh_verb_L{fs, patch_drums.hh_fdn_props, COMPONENT_HAS_GUI};
        fdn8_000 hh_verb_R{fs, patch_drums.hh_fdn_props, COMPONENT_HAS_GUI};
        peq_000 hh_peq{fs, patch_drums.hh_peq_props};

        cymbal_000 ride{fs, patch_drums.ride_props};
        peq_000 ride_peq{fs, patch_drums.ride_peq_props};

    } cymbals;

    struct {
        // 3 discrete lead instances with 2 voices each
        std::array<subtractive_synth_000, 6> lead_a{
                subtractive_synth_000{fs, patch_synth.lead_a_props},
                subtractive_synth_000{fs, patch_synth.lead_a_props},
                subtractive_synth_000{fs, patch_synth.lead_a_props},
                subtractive_synth_000{fs, patch_synth.lead_a_props},
                subtractive_synth_000{fs, patch_synth.lead_a_props},
                subtractive_synth_000{fs, patch_synth.lead_a_props}};
        std::array<nx_osc_000, 6> lead_b{
                nx_osc_000{fs, patch_synth.lead_b_props},
                nx_osc_000{fs, patch_synth.lead_b_props},
                nx_osc_000{fs, patch_synth.lead_b_props},
                nx_osc_000{fs, patch_synth.lead_b_props},
                nx_osc_000{fs, patch_synth.lead_b_props},
                nx_osc_000{fs, patch_synth.lead_b_props}};

        nx_osc_000 lead_b_lfo{fs, patch_synth.lead_b_lfo_props};

        peq_000 lead_peq{fs, patch_synth.lead_peq_props};

        fdn8_000 lead_verb_L{
                fs, patch_synth.chord_fdn_props.taps_mult(.495f).taps_add(82.f), COMPONENT_HAS_GUI};
        fdn8_000 lead_verb_R{
                fs, patch_synth.chord_fdn_props.taps_mult(.5f).taps_add(90.4f), COMPONENT_HAS_GUI};

        env_bp_000 autogain_lead_1;
        env_bp_000 autogain_lead_2;

        std::array<subtractive_synth_000, 6> chord{
                subtractive_synth_000{fs, patch_synth.chord_props},
                subtractive_synth_000{fs, patch_synth.chord_props},
                subtractive_synth_000{fs, patch_synth.chord_props},
                subtractive_synth_000{fs, patch_synth.chord_props},
                subtractive_synth_000{fs, patch_synth.chord_props},
                subtractive_synth_000{fs, patch_synth.chord_props}};

        peq_000 chord_preverb_peq{fs, patch_synth.chord_preverb_peq_props};
        fdn8_000 chord_verb_L{fs, patch_synth.chord_fdn_props, COMPONENT_HAS_GUI};
        fdn8_000 chord_verb_R{
                fs, patch_synth.chord_fdn_props.taps_mult(1.05f).taps_add(-22.f), COMPONENT_HAS_GUI};
        peq_000 chord_peq{fs, patch_synth.chord_peq_props};

        std::array<subtractive_synth_000, 12> pad{
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props},
                subtractive_synth_000{fs, patch_synth.pad_props}};

        peq_000 pad_preverb_peq{fs, patch_synth.pad_preverb_peq_props};
        fdn8_000 pad_verb_L{fs, patch_synth.pad_fdn_props, COMPONENT_HAS_GUI};
        fdn8_000 pad_verb_R{fs, patch_synth.pad_fdn_props.taps_add(9.345), COMPONENT_HAS_GUI};
        peq_000 pad_peq{fs, patch_synth.pad_peq_props};

    } synth;

    peq_000 master_peq{fs, patch_synth.master_peq_props};
};
