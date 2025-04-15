#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/mix.hpp"
#include "instruments/cymbal.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/nx_osc.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"

struct patch_drums_t {
    patch_drums_t();

    // --------------------------------
    // KICK

    kick_props kick_drum_props;
    fdn_8_props_019 kick_fdn_props;
    std::array<peq_props, peq::SIZE> kick_preverb_peq_props;
    std::array<peq_props, peq::SIZE> kick_peq_props;

    // --------------------------------
    // BASS

    subtractive_synth_props bass_props;
    std::array<peq_props, peq::SIZE> bass_peq_props;

    // --------------------------------
    // CYMBALS

    cymbal_props hh1_props;
    cymbal_props hh2_props;
    subtractive_synth_props hh3_props;
    fdn_8_props_019 hh_fdn_props;
    std::array<peq_props, peq::SIZE> hh_preverb_peq_props;
    std::array<peq_props, peq::SIZE> hh_peq_props;
    cymbal_props ride_props;
    std::array<peq_props, peq::SIZE> ride_peq_props;
};

struct patch_synth_t {
    patch_synth_t();

    // --------------------------------
    // LEAD

    subtractive_synth_props lead_a_props;
    nx_osc_props lead_b_props;
    nx_osc_props lead_b_lfo_props;
    std::array<peq_props, peq::SIZE> lead_peq_props{};

    // --------------------------------
    // CHORD

    subtractive_synth_props chord_props;
    fdn_8_props_019 chord_fdn_props;
    std::array<peq_props, peq::SIZE> chord_preverb_peq_props;
    std::array<peq_props, peq::SIZE> chord_peq_props;

    // --------------------------------
    // PAD

    subtractive_synth_props pad_props;
    std::array<peq_props, peq::SIZE> pad_preverb_peq_props;
    fdn_8_props_019 pad_fdn_props;
    std::array<peq_props, peq::SIZE> pad_peq_props;
};

struct patch_mix_t {
    patch_mix_t();

    composition_mix mix;

    // --------------------------------
    // MASTER

    std::array<peq_props, peq::SIZE> main_peq_props;
};

namespace patch {

extern patch_drums_t drums;
extern patch_synth_t synth;
extern patch_mix_t mix;

}  // namespace patch
