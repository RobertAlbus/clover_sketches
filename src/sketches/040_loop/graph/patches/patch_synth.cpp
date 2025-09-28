// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/fm/fm_037.hpp"
#include "lib/subtractive_synth/subtractive_synth_038.hpp"
#include "patches.hpp"

patch_synth_t::patch_synth_t() {
    // --------------------------------
    // BASS

    // clang-format off
    bass_fm_props = fm_props_037 {
            .tunings = {0.95, 1.12, 4, 0.5, 1.52, 1.47,},
            .tuning_types = {fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio},
            .mod_matrix = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.3, 0.1, 0, 0, 0, 0, 0.38, 0, 0.15,},
            .matrix_octave_range = 10,
            .amp_adsrs = {{{ 1111, 23427, 0, 1000 }, { 1111, 22755, 0, 1000 }, { 1, 2785, 0, 100 }, { 100, 1000, 1, 6721 }, { 1673, 1000, 1, 6721 }, { 1673, 1000, 1, 6721 }}},
            .pitch_adsrs = {{{ 100, 100, 1, 100 }, { 100, 1, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 1444, 0, 100 }, { 100, 1444, 0, 100 }}},
            .pitch_env_mod_depths = {0, 0, 0, 0, 2, 2,},
            .op_pans = {0, 0, 0, 0, -1, 1,},
            .op_output_gains = {0, 0, 0, 1, 0.41, 0.45,},
            .filter_type = filter_type::lp,
            .cut_adsr = { 865, 1444, 0.66, 964 },
            .res_adsr = { 1, 1, 0.004, 1 },
            .cut = 117.22743,
            .cut_mod_target = 453.97336,
            .res = 2.331,
            .res_mod_target = 2.331,
            .retrigger = true,
        };

    // clang-format on

    bass_preverb_peq_props = {
        peq_props_000{
            .freq    = 207,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 2813,
            .reso    = 0.727,
            .gain    = 24,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
    };

    bass_fdn_props = {
        .stereo_spread_taps_octaves = 0.288,
        .taps    = {83.14714, 66.62393, 56.258614, 64.65587, 52.506554, 72.91961, 58.531555, 53.025604},
        .fb_gain = 0.805,
        .lpf_cut = 13369.55,
        .lpf_res = 0.707,
        .hpf_cut = 34.435,
        .hpf_res = 0.707,
    };

    bass_postverb_peq_props = {
        peq_props_000{
            .freq    = 423,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 155,
            .reso    = 1.981,
            .gain    = -24,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 1657,
            .reso    = 1.562,
            .gain    = 4.992,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 7160,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
    };

    bass_peq_props = {
        peq_props_000{
            .freq    = 44,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 188,
            .reso    = 0.946,
            .gain    = -5.376,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 2335,
            .reso    = 0.269,
            .gain    = 6.528,
            .enabled = true,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = false,
            .type    = filter_type::lp,
        },
    };

    // --------------------------------
    // CHORD

    chord_1_props = {
        .tuning            = 0,
        .portamento_time   = 0,
        .pitch_env_octaves = 1,
        .retrigger         = true,
        .osc_tunings       = {-0.6, 0.6, -0.1, 0.1, 18.95, 19.05},
        .osc_pans          = {-1, 1, -1, 1, -1, 1},
        .osc_gains         = {1, 1, 1, 1, 0.52, 0.46},
        .waveforms =
            {waveform_000::saw,
             waveform_000::saw,
             waveform_000::saw,
             waveform_000::saw,
             waveform_000::square,
             waveform_000::square},
        .pitch_adsr      = {10, 10, 0, 10},
        .amp_adsr        = {808, 1000, 1, 1000},
        .cut_adsr        = {3460, 2000, 0.28, 100},
        .res_adsr        = {1, 1000, 0, 100},
        .cut             = 200,
        .res             = 1.07,
        .cut_env_octaves = 5,
        .res_env_octaves = 1.07,
        .filter_type     = filter_type::lp};

    chord_1_fdn_props = {
        .stereo_spread_taps_octaves = 0.458,
        .taps     = {1000.75696, 155.49821, 205.93652, 276.04752, 382.39215, 535.9393, 669.1728, 776.057},
        .fb_gain  = 0.808,
        .lpf_cut  = 4790.6606,
        .lpf_res  = 0.707,
        .hpf_cut  = 545.14856,
        .hpf_res  = 0.707,
    };

    std::array<peq_props_000, peq_000::SIZE> chord_1_peq_props{
    peq_props_000{
        .freq    = 309,
        .reso    = 0.707,
        .gain    = 0,
        .enabled = true,
        .type    = filter_type::hp,
    },
    peq_props_000{
        .freq    = 408,
        .reso    = 0.707,
        .gain    = 7.872,
        .enabled = true,
        .type    = filter_type::eq,
    },
    peq_props_000{
        .freq    = 6727,
        .reso    = 0.707,
        .gain    = 17.472,
        .enabled = true,
        .type    = filter_type::eq,
    },
    peq_props_000{
        .freq    = 3723,
        .reso    = 0.707,
        .gain    = 0,
        .enabled = true,
        .type    = filter_type::lp,
    },
};

    chord_2_props = {
        .tuning            = 0,
        .portamento_time   = 0,
        .pitch_env_octaves = 1,
        .retrigger         = true,
        .osc_tunings       = {-12.1, -11.9, -24.06, -23.94, 0, 0},
        .osc_pans          = {-1, 1, -1, 1, -1, 1},
        .osc_gains         = {1, 1, 1, 1, 0.52, 0.46},
        .waveforms =
            {waveform_000::square,
             waveform_000::square,
             waveform_000::square,
             waveform_000::square,
             waveform_000::noise,
             waveform_000::noise},
        .pitch_adsr      = {10, 10, 0, 10},
        .amp_adsr        = {1633, 1825, 0.46, 1633},
        .cut_adsr        = {1345, 5555, 0.48095232, 3047},
        .res_adsr        = {10, 1000, 0, 100},
        .cut             = 2897.6,
        .res             = 1,
        .cut_env_octaves = 4,
        .res_env_octaves = 0,
        .filter_type     = filter_type::lp};

    chord_2_fdn_props = {
        .stereo_spread_taps_octaves = 0.73,
        .taps    = {19842.588, 79.8482, 107.27557, 153.89427, 189.45346, 655.1626, 1891.7266, 5714.5605},
        .fb_gain = 0.996,
        .lpf_cut = 1231.6014,
        .lpf_res = 0.707,
        .hpf_cut = 126.09753,
        .hpf_res = 0.707,
    };

    std::array<peq_props_000, peq_000::SIZE> chord_2_peq_props{
        peq_props_000{
            .freq    = 140,
            .reso    = 1.782,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 328,
            .reso    = 0.329,
            .gain    = 7.68,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 11420,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
        peq_props_000{
            .freq    = 1764,
            .reso    = 0.707,
            .gain    = -6.912,
            .enabled = true,
            .type    = filter_type::hs,
        },
    };
};
