// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/math.hpp"

#include "lib/fdn/fdn8_stereo_030.hpp"
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
        .taps    = {56.145615, 54.151955, 54.808056, 275.67316, 521.6212, 558.33276, 863.30475, 1247.6532},
        .fb_gain = 0.984,
        .lpf_cut = 432.3778,
        .lpf_res = 0.707,
        .hpf_cut = 60.31463,
        .hpf_res = 0.707,
    };

    kick_preverb_peq_props = {
        peq_props_000{
            .freq    = 24,
            .reso    = 1.543,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
        peq_props_000{
            .freq    = 38,
            .reso    = 0.687,
            .gain    = 3.648,
            .enabled = true,
            .type    = peq_filter_type::eq,
        },
        peq_props_000{
            .freq    = 71,
            .reso    = 4.528,
            .gain    = -13.056,
            .enabled = false,
            .type    = peq_filter_type::eq,
        },
        peq_props_000{
            .freq    = 142,
            .reso    = 6.597,
            .gain    = -9.528,
            .enabled = true,
            .type    = peq_filter_type::eq,
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
            .freq    = 38,
            .reso    = 0.687,
            .gain    = 8.64,
            .enabled = true,
            .type    = peq_filter_type::eq,
        },
        peq_props_000{
            .freq    = 114,
            .reso    = 2.14,
            .gain    = -7.872,
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
    };

    // --------------------------------
    // SNARE
    snare_impulse_props = {
        .freqs            = {150, 404, 1533, 1751, 13456, 17500},
        .hpf_f0           = 20,
        .hpf_fmod_octaves = 0,
        .hpf_Q            = 0.852,
        .bpf_f0           = 979.2,
        .bpf_Q            = 0.576,
        .amp_a            = 1,
        .amp_d            = 500,
        .amp_s            = 0,
        .amp_r            = 3215.1787,
        .cut_a            = 1,
        .cut_d            = 1,
        .cut_s            = 0,
        .cut_r            = 1,
    };

    // original snare props
    snare_resonator_props = {
        .stereo_spread_taps_octaves = 0.208,
        .taps    = {640.97437, 278.1, 117.17493, 27.412, 24.742, 115.72901, 115.72901, 118.63938},
        .fb_gain = 0.968,
        .lpf_cut = 600.4853,
        .lpf_res = 0.707,
        .hpf_cut = 66.02632,
        .hpf_res = 0.707,
    };

    // wacked out snare props
    // gets right messed if you take the taps here against the kick pattern with the pads muted
    // snare_resonator_props = {
    //         .stereo_spread_taps_octaves = 1,
    //         .taps    = {414.81705, 278.1, 117.17493, 27.412, 24.742, 34096.543, 4150.826, 4384.0977},
    //         .fb_gain = 0.968,
    //         .lpf_cut = 546.06354,
    //         .lpf_res = 0.707,
    //         .hpf_cut = 59.447765,
    //         .hpf_res = 0.707,
    // };

    snare_driver_props = {
        .drive_input = 1.264,
        .drive_clip  = 4.712,
        .clip_thresh = 0.96,
        .trim        = 4.112,
    };

    snare_verb_props = {
        .stereo_spread_taps_octaves = 0.1,
        .taps    = {950.08716, 2529.9067, 1961.3395, 1749.114, 1445.2706, 1194.2994, 158.02867, 143.00116},
        .fb_gain = 0.899,
        .lpf_cut = 13254.815,
        .lpf_res = 0.707,
        .hpf_cut = 33.195488,
        .hpf_res = 0.707,
    };

    snare_peq_props = {
        peq_props_000{
            .freq    = 56,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = peq_filter_type::hp,
        },
        peq_props_000{
            .freq    = 65,
            .reso    = 6.656,
            .gain    = 23.616,
            .enabled = true,
            .type    = peq_filter_type::eq,
        },
        peq_props_000{
            .freq    = 200,
            .reso    = 6,
            .gain    = -7.872,
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
    };

    // --------------------------------
    // RIDE

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
