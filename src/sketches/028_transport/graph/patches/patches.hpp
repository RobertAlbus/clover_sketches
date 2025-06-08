#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/cymbal/cymbal_000.hpp"
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

struct patch {
    patch_drums_t drums;
    patch_synth_t synth;
    patch_mix_t mix;
};

/*
high reso eq boost (with cut mod) into this "kick" chain is mega heady and tribal

{
    .taps     = {617.7716, 687.3982, 797.0081, 17977.783, 22838.275, 259.32178, 60.076298, 54.269703},
    .fb_gain  = 0.95,
    .lpf_cut  = 1290.2051,
    .lpf_res  = 0.707,
    .hpf_cut  = 54.57113,
    .hpf_res  = 0.707,
};

{
            .trim              = 0.976,
            .drive             = 7.024,
            .amp_a             = 30,
            .amp_d             = 477.17914,
            .amp_s             = 0.23809522,
            .amp_r             = 298.3214,
            .cut_a             = 1,
            .cut_d             = 1524.3208,
            .cut_s             = 0,
            .cut_r             = 1.0000596,
            .filt_q            = 1.3999405,
            .pitch_a           = 1,
            .pitch_d           = 1690.4856,
            .pitch_s           = 0,
            .pitch_r           = 1000,
            .pitch_fundamental = 43.653526,
            .cut_fundamental   = 100,
            .pitch_range       = 1,
            .cut_range         = 4,
        };

*/