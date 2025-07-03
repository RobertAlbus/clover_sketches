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
        .trim              = 0.976,
        .drive             = 2.024,
        .amp_a             = 30,
        .amp_d             = 6191.322,
        .amp_s             = 0.08333331,
        .amp_r             = 1000,
        .cut_a             = 1,
        .cut_d             = 3905.1545,
        .cut_s             = 0,
        .cut_r             = 1.0000596,
        .filt_q            = 1.3999405,
        .pitch_a           = 1,
        .pitch_d           = 1690.4856,
        .pitch_s           = 0,
        .pitch_r           = 1000,
        .pitch_fundamental = midi_to_frequency(note::F2) / 2,
        .cut_fundamental   = 100,
        .pitch_range       = 1,
        .cut_range         = 3,
    };

    kick_fdn_props = {
        .taps    = {139.83583, 147.32317, 691.6512, 755.5922, 769.07587, 10995.189, 11804.371, 12231.032},
        .fb_gain = 0.923,
        .lpf_cut = 555.37897,
        .lpf_res = 0.707,
        .hpf_cut = 141.10242,
        .hpf_res = 0.707,
    };

    /*

    {
        .taps     = {84.76161, 90.78044, 95.58359, 153.78731, 10288.444, 10688.31, 11003.869, 11241.436},
        .fb_gain  = 0.87402976,
        .lpf_cut  = 792.2515,
        .lpf_res  = 0.707,
        .hpf_cut  = 181.85687,
        .hpf_res  = 0.707,
    };

    */
    kick_preverb_peq_props = {
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
        peq_props_000{
            .freq    = 373.2,
            .reso    = 1.2,
            .gain    = 8.1,
            .enabled = true,
            .type    = peq_filter_type::eq,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = peq_filter_type::lp,
        },
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = peq_filter_type::lp,
        },
    };

    kick_peq_props = {
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
        peq_props_000{
            .freq    = 81.3,
            .reso    = 1.1,
            .gain    = 6.3,
            .enabled = true,
            .type    = peq_filter_type::ls,
        },
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = peq_filter_type::lp,
        },
    };

    ride_props = {
        .freqs            = {150, 404, 1533, 1751, 13456, 17500},
        .hpf_f0           = 100,
        .hpf_fmod_octaves = 6,
        .hpf_Q            = 1,

        .bpf_f0 = 3000,
        .bpf_Q  = 1.3,

        .amp_a = 50,
        .amp_d = 500,
        .amp_s = 0.5,
        .amp_r = 25000,

        .cut_a = 60,
        .cut_d = 1000,
        .cut_s = 0.5,
        .cut_r = 2500000,
    };

    ride_peq_props = {
        peq_props_000{
            .freq    = 5938.7,
            .reso    = 0.9,
            .gain    = 14,
            .enabled = true,
            .type    = peq_filter_type::eq,
        },
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = peq_filter_type::lp,
        },
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
        peq_props_000{
            .freq    = 20,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
    };
}
