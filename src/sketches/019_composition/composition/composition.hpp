#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

#include "instruments/cymbal.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/nx_osc.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "patches/patch_drums.hpp"
#include "patches/patch_synth.hpp"

struct composition {
    std::pair<float, float> tick();

    static constexpr float fs = 48000;
    int fs_i                  = static_cast<int>(fs);
    int channel_count_out     = 2;
    double bpm                = 142;
    double spm                = fs * 60;
    double spb                = spm / bpm;
    double bar                = spb * 4;
    double beat               = bar / 4;
    int_fast64_t duration     = int_fast64_t(bar * 16 * 226) + 1;

    float gain_master = 0.5f;

    static patch_drums_t patch_drums;
    static patch_synth_t patch_synth;

    // semantically meaning label used as a flag for some instruments
    static constexpr bool COMPONENT_HAS_GUI = true;

    struct {
        kick_drum drum{fs, patch_drums.kick_drum_props};
        peq preverb_peq{fs, patch_drums.kick_preverb_peq_props};
        fdn_8_019 verb{fs, patch_drums.kick_fdn_props};
        peq out_peq{fs, patch_drums.kick_peq_props};
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
        alignas(64) fdn_8_019 hh_verb_L{fs, patch_drums.hh_fdn_props, COMPONENT_HAS_GUI};
        alignas(64) fdn_8_019 hh_verb_R{fs, patch_drums.hh_fdn_props, COMPONENT_HAS_GUI};
        peq hh_peq{fs, patch_drums.hh_peq_props};

        cymbal ride{fs, patch_drums.ride_props};
        peq ride_peq{fs, patch_drums.ride_peq_props};

    } cymbals;

    struct {
        // 3 discrete lead instances with 2 voices each
        std::array<subtractive_synth, 6> lead_a{
                subtractive_synth{fs, patch_synth.lead_props},
                subtractive_synth{fs, patch_synth.lead_props},
                subtractive_synth{fs, patch_synth.lead_props},
                subtractive_synth{fs, patch_synth.lead_props},
                subtractive_synth{fs, patch_synth.lead_props},
                subtractive_synth{fs, patch_synth.lead_props}};
        std::array<nx_osc, 6> lead_b{
                nx_osc{fs, patch_synth.lead_ringmod_props},
                nx_osc{fs, patch_synth.lead_ringmod_props},
                nx_osc{fs, patch_synth.lead_ringmod_props},
                nx_osc{fs, patch_synth.lead_ringmod_props},
                nx_osc{fs, patch_synth.lead_ringmod_props},
                nx_osc{fs, patch_synth.lead_ringmod_props}};

        peq lead_peq{fs, patch_synth.lead_peq_props};

        std::array<subtractive_synth, 6> chord{
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props},
                subtractive_synth{fs, patch_synth.chord_props}};

        peq chord_preverb_peq{fs, patch_synth.chord_preverb_peq_props};
        fdn_8_019 chord_verb_L{fs, patch_synth.chord_fdn_props};
        fdn_8_019 chord_verb_R{fs, patch_synth.chord_fdn_props};
        peq chord_peq{fs, patch_synth.chord_peq_props};

        std::array<subtractive_synth, 6> pad{
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props},
                subtractive_synth{fs, patch_synth.pad_props}};

        peq pad_preverb_peq{fs, patch_synth.pad_preverb_peq_props};
        fdn_8_019 pad_verb_L{fs, patch_synth.pad_fdn_props};
        fdn_8_019 pad_verb_R{fs, patch_synth.pad_fdn_props};
        peq pad_peq{fs, patch_synth.pad_peq_props};
    } synth;
};
