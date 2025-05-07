// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <print>

#include "clover/math.hpp"
#include "patches.hpp"
#include "sequence/notes.h"

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
                    .waveforms         = {waveform_000::square, waveform_000::square},
                    .retrigger         = true,
                    .pitch_a           = 10,
                    .pitch_d           = 10,
                    .pitch_s           = 0,
                    .pitch_r           = 10,
                    .amp_a             = 1500,
                    .amp_d             = 2000,
                    .amp_s             = 0.6,
                    .amp_r             = 1000,
            },
            .filter_props{
                    .cutoff               = 500,
                    .cutoff_range_octaves = 2,
                    .res                  = 1,
                    .res_range_octaves    = 0,
                    .filter_type          = filter_t_000::lpf,
                    .cut_a                = 1500,
                    .cut_d                = 1000,
                    .cut_s                = 0.1,
                    .cut_r                = 1000,
                    .res_a                = 10,
                    .res_d                = 1000,
                    .res_s                = 0,
                    .res_r                = 100,
            }};

    // chord_fdn_props = {
    //         .taps    = {137.289, 270.578, 337.222, 603.8, 937.022, 11000.334, 11533.489, 12000},
    //         .fb_gain = 0.918,
    //         .lpf_cut = 987.6031,
    //         .lpf_res = 0.707,
    //         .hpf_cut = 139.60605,
    //         .hpf_res = 0.707,
    // };

    chord_fdn_props = {
            .taps =
                    {
                            // clang-format off
                (48000 / midi_to_frequency(note::E3) *2) * (1 + (-2 * 0.251f)),
                (48000 / midi_to_frequency(note::E3) *2) * (1 + (-1 * 0.251f)),
                (48000 / midi_to_frequency(note::E3) *2) * (1 + (1 * 0.251f)),
                (48000 / midi_to_frequency(note::E3) *2) * (1 + (2 * 0.251f)),

                ((48000 / midi_to_frequency(note::E3) * 32)) * (1 +  (-2 * 0.01f)),
                ((48000 / midi_to_frequency(note::E3) * 32)) * (1 +  (-1 * 0.01f)),
                ((48000 / midi_to_frequency(note::E3) * 32)) * (1 +  (1 * 0.01f)),
                ((48000 / midi_to_frequency(note::E3) * 32)) * (1 +  (2 * 0.01f)),
            },
            // clang-format on
            .fb_gain = 0.918,
            .lpf_cut = 3052.002,
            .lpf_res = 0.707,
            .hpf_cut = 139.60605,
            .hpf_res = 0.707,
    };

    std::array<peq_props_000, peq_000::SIZE> chord_preverb_peq_props{
            peq_props_000{
                    .freq    = 5657.1,
                    .reso    = 3,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props_000{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props_000{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props_000{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };

    chord_peq_props = {
            peq_props_000{
                    .freq    = 180.1,
                    .reso    = .707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };

    echoverb_props             = {.delay_samples = 11612.903, .fb = 1.154};
    chord_echoverb_fdn_L_props = {
            .taps    = {134.42862, 478.418, 613.966, 1562.802, 1833.898, 2443.864, 2782.734, 3596.022},
            .fb_gain = 0.931,
            .lpf_cut = 1004.95825,
            .lpf_res = 0.707,
            .hpf_cut = 89.94652,
            .hpf_res = 0.407,
    };
    chord_echoverb_fdn_R_props = {
            .taps    = {1562.802, 1630.576, 2037.22, 2511.638, 3460.475, 3934.893, 6781.401, 8543.525},
            .fb_gain = 0.919,
            .lpf_cut = 1045.5166,
            .lpf_res = 0.707,
            .hpf_cut = 114.046616,
            .hpf_res = 0.407,
    };

    chord_echoverb_peq_props = {
            peq_props_000{
                    .freq    = 6099.6,
                    .reso    = 0.6,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props_000{
                    .freq    = 248.1,
                    .reso    = 0.6,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props_000{
                    .freq    = 477,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::notch,
            },
            peq_props_000{
                    .freq    = 1599.2,
                    .reso    = 0.5,
                    .gain    = -3.2,
                    .enabled = true,
                    .type    = peq_filter_type::eq,
            },
    };
};
