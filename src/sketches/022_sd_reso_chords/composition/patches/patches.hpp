#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/mix.hpp"
#include "instruments/022_echoverb.hpp"
#include "instruments/fdn.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "lib/kick_drum/kick_drum.hpp"

struct patch_drums_t {
    patch_drums_t();

    // --------------------------------
    // KICK

    kick_props_000 kick_drum_props;
    fdn_8_props_019 kick_fdn_props;
    std::array<peq_props, peq::SIZE> kick_preverb_peq_props;
    std::array<peq_props, peq::SIZE> kick_peq_props;
};

struct patch_synth_t {
    patch_synth_t();

    // --------------------------------
    // CHORD

    subtractive_synth_props chord_props;
    fdn_8_props_019 chord_fdn_props;
    std::array<peq_props, peq::SIZE> chord_preverb_peq_props;
    std::array<peq_props, peq::SIZE> chord_peq_props;

    echoverb_022_props echoverb_props;
    std::array<peq_props, peq::SIZE> chord_echoverb_peq_props;
    fdn_8_props_019 chord_echoverb_fdn_L_props;
    fdn_8_props_019 chord_echoverb_fdn_R_props;
};

struct patch_mix_t {
    patch_mix_t();

    std::vector<mixer_track> mixer_tracks;

    // --------------------------------
    // MASTER

    std::array<peq_props, peq::SIZE> main_peq_props;
};

namespace patch {

extern patch_drums_t drums;
extern patch_synth_t synth;
extern patch_mix_t mix;

}  // namespace patch
