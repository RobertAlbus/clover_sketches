// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/math.hpp"

#include "patches.hpp"
#include "sequence/notes.h"

patch_drums_t::patch_drums_t() {
    // --------------------------------
    // KICK
    kick_drum_props = {
        .trim              = 0.512,
        .drive             = 8.638,
        .amp_a             = 30,
        .amp_d             = 5000,
        .amp_s             = 0.25475892,
        .amp_r             = 1000,
        .cut_a             = 1,
        .cut_d             = 3905.1545,
        .cut_s             = 0,
        .cut_r             = 1.0000596,
        .filt_q            = 1.3999405,
        .pitch_a           = 1,
        .pitch_d           = 4071.2004,
        .pitch_s           = 0,
        .pitch_r           = 1000,
        .pitch_fundamental = midi_to_frequency(note::Fs2) / 2,
        .cut_fundamental   = 100,
        .pitch_range       = 2,
        .cut_range         = 3,
    };

    kick_fdn_props = {
        .taps    = {134.42862, 329.99323, 721.1825, 1308.0359, 1959.9309, 3329.2222, 6458.37, 11087.26},
        .fb_gain = 0.87402976,
        .lpf_cut = 180.31549,
        .lpf_res = 0.707,
        .hpf_cut = 45.100502,
        .hpf_res = 0.707,
    };

    kick_preverb_peq_props = {
        peq_props_000{
            .freq    = 20,
            .reso    = .707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 20,
            .reso    = .707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
    };

    kick_peq_props = {
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 81.3,
            .reso    = 1.1,
            .gain    = 6.3,
            .enabled = true,
            .type    = filter_type::ls,
        },
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
    };
}
