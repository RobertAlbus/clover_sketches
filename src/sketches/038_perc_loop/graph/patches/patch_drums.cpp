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
        .trim              = 1,
        .drive             = 3.571,
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
        .cut_range         = 5,
    };

    kick_fdn_props = {
        .taps     = {62.12766, 223.88464, 240.89304, 248.57938, 452.64426, 628.02136, 79.04517, 62.749657}, 
        .fb_gain  = 0.96, 
        .lpf_cut  = 2252.4702, 
        .lpf_res  = 0.707, 
        .hpf_cut  = 40.044914, 
        .hpf_res  = 0.707, 
    };

    kick_preverb_peq_props = {
        peq_props_000{
            .freq    = 24,
            .reso    = 1.543,
            .gain    = 0,
            .enabled = false,
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
            .freq    = 71,
            .reso    = 4.528,
            .gain    = -13.056,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 449,
            .reso    = 2,
            .gain    = 11.712,
            .enabled = false,
            .type    = filter_type::eq,
        },
    };

    kick_peq_props = {
        peq_props_000{
            .freq    = 16,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 38,
            .reso    = 0.687,
            .gain    = 8.64,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 3273,
            .reso    = 0.169,
            .gain    = 2.112,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 3274,
            .reso    = 3.831,
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
    snare_body_resonator_props = {
        .stereo_spread_taps_octaves = 0.068,
        .taps    = {45.860977, 71.467995, 73.65708, 53.5502, 10934.775, 123.870575, 126.44967, 126.44967},
        .fb_gain = 0.978,
        .lpf_cut = 1012.182,
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
        .trim        = 4.112,
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
        .pitch_a         = 10,
        .pitch_d         = 2000,
        .pitch_s         = 0,
        .pitch_r         = 1000,
        .amp_a           = 10,
        .amp_d           = 2222,
        .amp_s           = 0,
        .amp_r           = 1000,
        .cut             = 100,
        .res             = 0.6,
        .cut_env_octaves = 10,
        .res_env_octaves = 0,
        .filter_type     = filter_type::lp,
        .cut_a           = 10,
        .cut_d           = 3333,
        .cut_s           = 0,
        .cut_r           = 1000,
        .res_a           = 10,
        .res_d           = 1000,
        .res_s           = 0,
        .res_r           = 100};

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
}
