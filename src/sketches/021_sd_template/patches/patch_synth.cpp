// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patches.hpp"

patch_synth_t::patch_synth_t() {
    // --------------------------------
    // CHORD

    chord_props = {
            .osc_props{
                    .tuning            = 0,
                    .portamento_time   = 0,
                    .pitch_env_octaves = 0,
                    .osc_tunings       = {0.2, 7.2},
                    .osc_pans          = {-1.f, 1.0f},
                    .waveforms         = {waveform::square, waveform::square},
                    .retrigger         = true,
                    .pitch_a           = 10,
                    .pitch_d           = 10,
                    .pitch_s           = 0,
                    .pitch_r           = 10,
                    .amp_a             = 100,
                    .amp_d             = 2000,
                    .amp_s             = 0.6,
                    .amp_r             = 1000,
            },
            .filter_props{
                    .cutoff               = 500,
                    .cutoff_range_octaves = 2,
                    .res                  = 1,
                    .res_range_octaves    = 0,
                    .filter_type          = filter_t::lpf,
                    .cut_a                = 10,
                    .cut_d                = 1000,
                    .cut_s                = 0.1,
                    .cut_r                = 1000,
                    .res_a                = 10,
                    .res_d                = 1000,
                    .res_s                = 0,
                    .res_r                = 100,
            }};

    chord_fdn_props = {
            .taps    = {2090.261, 2285.848, 2677.022, 3524.565, 4502.5, 5415.239, 6197.587, 6653.957},
            .fb_gain = 0.918,
            .lpf_cut = 3052,
            .lpf_res = 0.707,
            .hpf_cut = 91,
            .hpf_res = 0.707,
    };

    std::array<peq_props, peq::SIZE> chord_preverb_peq_props{
            peq_props{
                    .freq    = 5657.1,
                    .reso    = 3,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };

    chord_peq_props = {
            peq_props{
                    .freq    = 180.1,
                    .reso    = .707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };
};
