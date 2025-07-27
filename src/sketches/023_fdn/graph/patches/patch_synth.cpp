// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <print>

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

    chord_fdn_props_L = {
        .taps    = {70.644, 403.867, 1536.822, 2936.355, 3136.289, 4869.044, 7801.4, 10733.756},
        .fb_gain = 0.918,
        .lpf_cut = 6391.762,
        .lpf_res = 0.707,
        .hpf_cut = 139.60605,
        .hpf_res = 0.707,
    };
    chord_fdn_props_R = chord_fdn_props_L.taps_mult(12.f / 7.f).taps_add(-22.5f);

    std::array<peq_props_000, peq_000::SIZE> chord_preverb_peq_props{
        peq_props_000{
            .freq    = 3186.4,
            .reso    = 0.3,
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
};
