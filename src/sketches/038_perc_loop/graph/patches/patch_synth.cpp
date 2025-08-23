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
        {
            .tunings = {0.95, 1.12, 1, 0.5, 5, 12,},
            .tuning_types = {fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio, fm_tuning_type::ratio},
            .mod_matrix = {1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
            .matrix_octave_range = 5,
            .amp_adsrs = {{{ 1, 12099.5, 0, 4227.828 }, { 1, 12579.479, 0, 100 }, { 100, 100, 1, 100 }, { 100, 24000, 0.004, 10659.561 }, { 14019.421, 100, 1, 100 }, { 8355.656, 100, 1, 100 }}},
            .pitch_adsrs = {{{ 100, 100, 1, 100 }, { 100, 1, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }, { 100, 100, 1, 100 }}},
            .pitch_env_mod_depths = {0, 0, 0, 0, 0, 0,},
            .op_pans = {-1, 1, 0, 0, 0, 0,},
            .op_output_gains = {1, 1, 0, 1, 0, 0,},
            .filter_type = filter_type::lp,
            .cut_adsr = { 288.988, 5091.792, 0.38400003, 100 },
            .res_adsr = { 1, 1, 0.004, 1 },
            .cut = 20,
            .cut_mod_target = 640,
            .res = 3.36,
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
        .osc_tunings       = {0.2, 7.2, 0, 0, 0, 0},
        .osc_pans          = {-1, 1, -1, 1, 0, 0},
        .osc_gains         = {1, 1, 0.1, 0.1, 0, 0},
        .waveforms =
            {waveform_000::square,
             waveform_000::square,
             waveform_000::noise,
             waveform_000::noise,
             waveform_000::sine,
             waveform_000::sine},
        .pitch_adsr      = {10, 10, 0, 10},
        .amp_adsr        = {3500, 2000, 0.6, 1000},
        .cut_adsr        = {3500, 1000, 0.1, 1000},
        .res_adsr        = {10, 1000, 0, 100},
        .cut             = 500,
        .res             = 1,
        .cut_env_octaves = 2,
        .res_env_octaves = 0,
        .filter_type     = filter_type::lp,
    };

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
        .amp_adsr        = {100, 4000, 0, 100},
        .cut_adsr        = {100, 2000, 0.48095232, 100},
        .res_adsr        = {10, 1000, 0, 100},
        .cut             = 500,
        .res             = 1,
        .cut_env_octaves = 3.08,
        .res_env_octaves = 0,
        .filter_type     = filter_type::lp};

    chord_fdn_props = {
        .stereo_spread_taps_octaves = 0.458,
        .taps    = {1012.4044, 287.87997, 290.91763, 300.22708, 698.1039, 713.03784, 150.73712, 158.75778},
        .fb_gain = 0.9,
        .lpf_cut = 1566.8373,
        .lpf_res = 0.707,
        .hpf_cut = 71.12249,
        .hpf_res = 0.707,
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
