#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/cymbal/cymbal.hpp"
#include "lib/fdn/fdn8_023.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "lib/mixer/mixer.hpp"
#include "lib/peq/peq.hpp"
#include "lib/subtractive_synth/subtractive_synth.hpp"

struct patch_drums_t {
    patch_drums_t();

    // --------------------------------
    // KICK

    kick_props_000 kick_drum_props;
    fdn8_props_023 kick_fdn_props;
    std::array<peq_props_000, peq_000::SIZE> kick_preverb_peq_props;
    std::array<peq_props_000, peq_000::SIZE> kick_peq_props;

    // --------------------------------
    // RIDE
    std::array<peq_props_000, peq_000::SIZE> ride_peq_props;
    cymbal_props_000 ride_props;
};

struct patch_synth_t {
    patch_synth_t();

    // --------------------------------
    // CHORD

    subtractive_synth_props_000 chord_props;
    fdn8_props_023 chord_fdn_props_L;
    fdn8_props_023 chord_fdn_props_R;

    std::array<peq_props_000, peq_000::SIZE> chord_preverb_peq_props;
    std::array<peq_props_000, peq_000::SIZE> chord_peq_props;
};

struct patch_mix_t {
    patch_mix_t();

    std::vector<mixer_track_000> mixer_tracks;

    // --------------------------------
    // MASTER

    std::array<peq_props_000, peq_000::SIZE> main_peq_props;
};

namespace patch {

extern patch_drums_t drums;
extern patch_synth_t synth;
extern patch_mix_t mix;

}  // namespace patch
