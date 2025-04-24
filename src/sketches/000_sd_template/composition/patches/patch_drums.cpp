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
            .trim              = 0.84297022,
            .drive             = 2.6860298,
            .amp_a             = 30,
            .amp_d             = 5000,
            .amp_s             = 0.25475893,
            .amp_r             = 1000,
            .cut_a             = 1,
            .cut_d             = 5809.8223,
            .cut_s             = 0,
            .cut_r             = 1.0000596,
            .filt_q            = 1.3999405,
            .pitch_a           = 1,
            .pitch_d           = 2999.8784,
            .pitch_s           = 0,
            .pitch_r           = 1000,
            .pitch_fundamental = midi_to_frequency(note::F2) / 2,
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
            peq_props{
                    .freq    = 20,
                    .reso    = .707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = .707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };

    kick_peq_props = {
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 41,
                    .reso    = 1.1,
                    .gain    = -2.0,
                    .enabled = true,
                    .type    = peq_filter_type::ls,
            },
            peq_props{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };
}
