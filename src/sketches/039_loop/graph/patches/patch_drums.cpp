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

    // midi_to_frequency(note::F2) / 2,

    kick_drum_props = {
        .trim              = 1,
        .drive             = 5.67,
        .amp_adsr          = {30, 6191.322, 0, 884},
        .pitch_adsr        = {1, 2081, 0, 1000},
        .cut_adsr          = {1, 4945, 0, 1.0000596},
        .filt_q            = 2.19,
        .pitch_fundamental = midi_to_frequency(note::F2) / 2,
        .cut_fundamental   = 70,
        .pitch_range       = 1,
        .cut_range         = 1.46,
    };

    kick_preverb_peq_props = {
        peq_props_000{
            .freq    = 1046,
            .reso    = 4.608,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
        peq_props_000{
            .freq    = 38,
            .reso    = 0.687,
            .gain    = 3.648,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 49,
            .reso    = 4.528,
            .gain    = 24,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1377,
            .reso    = 2,
            .gain    = 24,
            .enabled = false,
            .type    = filter_type::eq,
        },
    };

    kick_fdn_props = {
        .taps    = {62.12766, 80.65981, 90.18173, 98.8457, 452.64426, 641.44763, 79.04517, 8652.805},
        .fb_gain = 0.96,
        .lpf_cut = 8501.097,
        .lpf_res = 0.707,
        .hpf_cut = 26.642181,
        .hpf_res = 0.707,
    };

    kick_postverb_peq_props = {
        peq_props_000{
            .freq    = 81,
            .reso    = 1.284,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 38,
            .reso    = 0.687,
            .gain    = 3.648,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1068,
            .reso    = 0.628,
            .gain    = 9.216,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1377,
            .reso    = 2,
            .gain    = 24,
            .enabled = false,
            .type    = filter_type::eq,
        },
    };

    kick_peq_props = {
        peq_props_000{
            .freq    = 37,
            .reso    = 3.931,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 204,
            .reso    = 1.363,
            .gain    = -6.144,
            .enabled = true,
            .type    = filter_type::ls,
        },
        peq_props_000{
            .freq    = 3273,
            .reso    = 0.169,
            .gain    = 2.112,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 4468,
            .reso    = 1.443,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
    };

    // --------------------------------
    // SNARE
    snare_body_impulse_props = {
        .freqs            = {150, 404, 1533, 1751, 13456, 17500},
        .hpf_f0           = 20,
        .hpf_fmod_octaves = 0,
        .hpf_Q            = 0.852,
        .bpf_f0           = 979.2,
        .bpf_Q            = 0.576,
        .amp_adsr         = {1, 500, 0, 3215.1787},
        .cut_adsr         = {1, 1, 0, 1},
    };

    // original snare props
    snare_body_resonator_props = {
        .stereo_spread_taps_octaves = 0.068,
        .taps    = {67.29404, 105.10034, 205.93652, 1952.976, 264.70828, 114.08613, 65.30492, 57.954212},
        .fb_gain = 0.978,
        .lpf_cut = 643.6013,
        .lpf_res = 0.707,
        .hpf_cut = 67.91454,
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

    snare_body_driver_props = {
        .drive_input = 1.264,
        .drive_clip  = 4.712,
        .clip_thresh = 0.96,
        .trim        = 10,
    };

    snare_body_peq_props = {
        peq_props_000{
            .freq    = 118,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 65,
            .reso    = 6.656,
            .gain    = 24,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 653,
            .reso    = 2.02,
            .gain    = -4.8,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 2182,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
    };

    snare_noise_props = {
        .tuning            = 0,
        .portamento_time   = 0,
        .pitch_env_octaves = 3,
        .retrigger         = true,
        .osc_tunings       = {0.2, 7.2, -27, 0, 0, 0},
        .osc_pans          = {-0.7, 0.45, 0, 0, 0, 0},
        .osc_gains         = {1, 1, 1, 0, 0, 0},
        .waveforms =
            {waveform_000::noise,
             waveform_000::noise,
             waveform_000::sine,
             waveform_000::sine,
             waveform_000::sine,
             waveform_000::sine},
        .pitch_adsr      = {10, 2000, 0, 1000},
        .amp_adsr        = {10, 2222, 0, 1000},
        .cut_adsr        = {10, 3333, 0, 1000},
        .res_adsr        = {10, 1000, 0, 100},
        .cut             = 100,
        .res             = 0.6,
        .cut_env_octaves = 10,
        .res_env_octaves = 0,
        .filter_type     = filter_type::lp};

    snare_noise_peq_props = {
        peq_props_000{
            .freq    = 42,
            .reso    = 0.9,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 62,
            .reso    = 2.418,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 441,
            .reso    = 0.985,
            .gain    = 7.104,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1368,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
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

        .amp_adsr = {50, 500, 0.5, 25000},
        .cut_adsr = {60, 1000, 0.5, 2500000},
    };

    ride_peq_props = {
        peq_props_000{
            .freq    = 5938.7,
            .reso    = 0.9,
            .gain    = 14,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
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
    };

    hh1_props = {
        .freqs            = {31, 82, 215, 227, 248, 278},
        .hpf_f0           = 20,
        .hpf_fmod_octaves = 2.76,
        .hpf_Q            = 5.119,
        .bpf_f0           = 6453.12,
        .bpf_Q            = 2.409,
        .amp_adsr         = {50, 1000, 0, 1786.6552},
        .cut_adsr         = {60, 274.53574, 0, 1072.3923},
    };

    hh1_driver_props = {
        .drive_input = 5.488,
        .drive_clip  = 8,
        .clip_thresh = 0.04,
        .trim        = 4.96,
    };

    hh1_peq_props = {
        peq_props_000{
            .freq    = 257,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 2342,
            .reso    = 4.448,
            .gain    = 17.664,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1565,
            .reso    = 7.627,
            .gain    = 16.668,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 7826,
            .reso    = 5.443,
            .gain    = 23.616,
            .enabled = true,
            .type    = filter_type::eq,
        },
    };

    hh2_props = {
        .freqs            = {100, 150, 175, 225, 66.6, 0},
        .hpf_f0           = 20,
        .hpf_fmod_octaves = 5.976,
        .hpf_Q            = 2.386,
        .bpf_f0           = 5877.6,
        .bpf_Q            = 4.31,
        .amp_adsr         = {50, 1000, 0, 1},
        .cut_adsr         = {60, 1000, 0, 1},
    };

    hh2_driver_props = {
        .drive_input = 1.264,
        .drive_clip  = 4.712,
        .clip_thresh = 0.96,
        .trim        = 4.112,
    };

    hh2_peq_props = {
        peq_props_000{
            .freq    = 249,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 1010,
            .reso    = 6.637,
            .gain    = 10.176,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1471,
            .reso    = 6.633,
            .gain    = 15.744,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 2324,
            .reso    = 7.592,
            .gain    = 15.168,
            .enabled = true,
            .type    = filter_type::eq,
        },
    };

    // hh3_props = {
    //     .freqs            = {2009.6758, 10293.28, 5536.9336, 7529.0117, 17366.127, 20748.287},
    //     .hpf_f0           = 1554.72,
    //     .hpf_fmod_octaves = 5.976,
    //     .hpf_Q            = 10,
    //     .bpf_f0           = 7220.481,
    //     .bpf_Q            = 10,
    //     .amp_adsr         = {1, 1740, 0.08, 6361},
    //     .cut_adsr         = {60, 204, 0, 11111},
    // };

    hh3_props = {
        .freqs            = {7057.3027, 1794.6157, 1843.6337, 1914.5178, 2109.565, 2387.994},
        .hpf_f0           = 691.44,
        .hpf_fmod_octaves = 2.664,
        .hpf_Q            = 2.951,
        .bpf_f0           = 4822.48,
        .bpf_Q            = 3.031,
        .amp_adsr         = {50, 625, 0.28, 601},
        .cut_adsr         = {60, 433, 0.48, 193},
    };

    hh3_peq_props = {
        peq_props_000{
            .freq    = 614,
            .reso    = 6.896,
            .gain    = 13.056,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 2632,
            .reso    = 9.343,
            .gain    = -16.128,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 3298,
            .reso    = 2.936,
            .gain    = 15.168,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 2202,
            .reso    = 4.209,
            .gain    = 14.016,
            .enabled = true,
            .type    = filter_type::eq,
        },
    };
}
