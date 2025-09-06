// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/fm/fm_037.hpp"
#include "patches.hpp"

patch_synth_t::patch_synth_t() {
    // --------------------------------
    // BASS

    // clang-format off
    bass_fm_props = fm_props_037
        // {
        //     .tunings = {0.95, 1.12, 1, 0.5, 5, 12,},
        //     .tuning_types = {fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio},
        //     .mod_matrix = {1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        //     .matrix_octave_range = 5,
        //     .amp_adsrs = {{{ 1, 12099.5, 0, 4227.828 }, { 1, 12579.479, 0, 100 }, { 100, 100, 1, 100 }, { 100, 24000, 0.004, 10659.561 }, { 14019.421, 100, 1, 100 }, { 8355.656, 100, 1, 100 }}},
        //     .pitch_adsrs = {{{ 100, 100, 1, 100 }, { 100, 1, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }}},
        //     .pitch_env_mod_depths = {0, 0, 0, 0, 0, 0,},
        //     .op_pans = {-1, 1, 0, 0, 0, 0,},
        //     .op_output_gains = {1, 1, 0, 1, 0, 0,},
        //     .filter_type = filter_type::lp,
        //     .cut_adsr = { 288.988, 5091.792, 0.38400003, 100 },
        //     .res_adsr = { 1, 1, 0.004, 1 },
        //     .cut = 31.119287,
        //     .cut_mod_target = 507.02576,
        //     .res = 2.331,
        //     .res_mod_target = 10,
        //     .retrigger = true,
        // };
        /* transition to this when chord verb grows big */
        {
            .tunings = {0.95, 1.12, 1, 0.5, 5, 12,},
            .tuning_types = {fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio},
            .mod_matrix = {1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
            .matrix_octave_range = 10,
            .amp_adsrs = {{{ 1, 23427, 0, 4227.828 }, { 1, 22755, 0, 100 }, { 100, 100, 1, 100 }, { 100, 24000, 0.004, 10659.561 }, { 1441, 100, 1, 100 }, { 1252, 100, 1, 100 }}},
            .pitch_adsrs = {{{ 100, 100, 1, 100 }, { 100, 1, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }}},
            .pitch_env_mod_depths = {0, 0, 0, 0, 0, 0,},
            .op_pans = {-1, 1, 0, 0, 0, 0,},
            .op_output_gains = {1, 1, 0, 1, 0, 0,},
            .filter_type = filter_type::lp,
            .cut_adsr = { 8641, 5091.792, 0.38400003, 100 },
            .res_adsr = { 1, 1, 0.004, 1 },
            .cut = 31.119287,
            .cut_mod_target = 650.1747,
            .res = 2.331,
            .res_mod_target = 10,
            .retrigger = true,
        };

    // clang-format on

    // --------------------------------
    // CHORD

    chord_props = {
        .tuning            = 0,
        .portamento_time   = 0,
        .pitch_env_octaves = 1,
        .retrigger         = true,
        .osc_tunings       = {0, 0, 12, 12, 0, 0},
        .osc_pans          = {-1, 1, -1, 1, -1, 1},
        .osc_gains         = {1, 1, 1, 1, 0.52, 0.46},
        .waveforms =
            {waveform_000::square,
             waveform_000::square,
             waveform_000::saw,
             waveform_000::saw,
             waveform_000::noise,
             waveform_000::noise},
        .pitch_adsr      = {10, 10, 0, 10},
        .amp_adsr        = {100, 5056, 0.15, 100},
        .cut_adsr        = {100, 2000, 0.48095232, 100},
        .res_adsr        = {10, 1000, 0, 100},
        .cut             = 500,
        .res             = 1,
        .cut_env_octaves = 2.98,
        .res_env_octaves = 0,
        .filter_type     = filter_type::lp,
    };

    // chord_props = {
    //     .tuning            = 0,
    //     .portamento_time   = 0,
    //     .pitch_env_octaves = 1,
    //     .retrigger         = true,
    //     .osc_tunings       = {0, 0, 12, 12, 0, 0},
    //     .osc_pans          = {-1, 1, -1, 1, -1, 1},
    //     .osc_gains         = {1, 1, 1, 1, 0.52, 0.46},
    //     .waveforms =
    //         {waveform_000::square,
    //          waveform_000::square,
    //          waveform_000::saw,
    //          waveform_000::saw,
    //          waveform_000::noise,
    //          waveform_000::noise},
    //     .pitch_adsr      = {10, 10, 0, 10},
    //     .amp_adsr        = {100, 4000, 0, 100},
    //     .cut_adsr        = {100, 2000, 0.48095232, 100},
    //     .res_adsr        = {10, 1000, 0, 100},
    //     .cut             = 500,
    //     .res             = 1,
    //     .cut_env_octaves = 3.08,
    //     .res_env_octaves = 0,
    //     .filter_type     = filter_type::lp};

    // chord_fdn_props = {
    //     .stereo_spread_taps_octaves = 0.458,
    //     .taps    = {1012.00006, 287.87997, 290.91763, 300.22708, 698.1039, 713.03784, 150.73712,
    //     158.75778}, .fb_gain = 0.823, .lpf_cut = 1521.3751, .lpf_res = 0.707, .hpf_cut = 71.12249, .hpf_res
    //     = 0.707,
    // };
    // walk through taps[0] = 1012, 1022, 1033, 103

    /*
    later:
    lerp toward these props to grow a big wash in the back 9, and pump it
    */
    chord_fdn_props = {
        .stereo_spread_taps_octaves = 0.458,
        .taps                       = {32238.998, 16120, 870, 435, 217.49998, 108, 54.000004, 27},
        .fb_gain                    = 0.847,
        .lpf_cut                    = 4182.1816,
        .lpf_res                    = 0.707,
        .hpf_cut                    = 71.12249,
        .hpf_res                    = 0.707,
    };

    std::array<peq_props_000, peq_000::SIZE> chord_preverb_peq_props{
        peq_props_000{
            .freq    = 207,
            .reso    = 0.707,
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
};
