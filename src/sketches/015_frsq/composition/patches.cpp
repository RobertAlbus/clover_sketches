// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/patches.hpp"
#include "composition/instruments/nx_osc.hpp"

fdn_8_props_012 patch_deep_fdn = {
        .taps    = {134.42862, 329.99323, 721.1825, 1308.0359, 1959.9309, 3329.2222, 6458.37, 11087.26},
        .fb_gain = 0.87402976,
        .lpf_cut = 180.31549,
        .lpf_res = 0.707,
        .hpf_cut = 45.100502,
        .hpf_res = 0.707,
};

kick_props patch_deep_kick{
        .trim              = 0.6429702,
        .drive             = 0.6860298,
        .amp_a             = 30,
        .amp_d             = 5000,
        .amp_s             = 0.5475893,
        .amp_r             = 1000,
        .cut_a             = 1,
        .cut_d             = 5809.8223,
        .cut_s             = 0,
        .cut_r             = 1.0000596,
        .filt_q            = 1.3999405,
        .pitch_a           = 1,
        .pitch_d           = 2999.8784,
        .pitch_s           = 6.99e-43,
        .pitch_r           = 1000,
        .pitch_fundamental = 40.001907,
        .pitch_peak        = 200,
        .cut_fundamental   = 100,
        .cut_peak          = 235,
        .pitch_range       = 2.3218594,
        .cut_range         = 2.0249748,
};
std::vector<settable> osc_tunings;  // semitones.cents, relative
std::vector<settable> osc_pans;     // [L,R] = [-1,1]
std::vector<settable_int> waveforms_i;

nx_osc_props patch_deep_chord{
        .tuning            = -24,
        .portamento_time   = 0,
        .pitch_env_octaves = 0,
        .osc_tunings       = {0, 12.10f, 11.90f},
        .osc_pans          = {0, -0.3f, 0.3f},
        .waveforms_i       = {int(waveform::sine), int(waveform::saw), int(waveform::saw)},
        .pitch_a           = 100,
        .pitch_d           = 100,
        .pitch_s           = 0,
        .pitch_r           = 1000,
        .amp_a             = 100,
        .amp_d             = 1000,
        .amp_s             = 0.1,
        .amp_r             = 1000,
};

filter_block_props patch_deep_chord_filter{
        .cutoff               = 500,
        .cutoff_range_octaves = 2.5,
        .res                  = 0.707,
        .res_range_octaves    = 0,
        .filter_type_i        = int(filter_t::lpf),
        .cut_a                = 5000,
        .cut_d                = 8000,
        .cut_s                = 0.5,
        .cut_r                = 1000,
        .res_a                = 1000,
        .res_d                = 1000,
        .res_s                = 0.1,
        .res_r                = 1000,
};

fdn_8_props_012 patch_real_fdn = {
        .taps    = {134.42862, 329.99323, 721.1825, 395.18927, 916.76953, 525.59546, 525.59546, 395.18927},
        .fb_gain = 0.9289702,
        .lpf_cut = 149.90222,
        .lpf_res = 0.707,
        .hpf_cut = 14.883215,
        .hpf_res = 0.707,
};

kick_props patch_real_kick{
        .trim              = 0.6429702,
        .drive             = 0.6860298,
        .amp_a             = 30,
        .amp_d             = 2619.231,
        .amp_s             = 6.99e-43,
        .amp_r             = 1000,
        .cut_a             = 1,
        .cut_d             = 5809.8223,
        .cut_s             = 0,
        .cut_r             = 1.0000596,
        .filt_q            = 1.3999405,
        .pitch_a           = 1,
        .pitch_d           = 1452.4756,
        .pitch_s           = 6.99e-43,
        .pitch_r           = 1000,
        .pitch_fundamental = 46.001907,
        .pitch_peak        = 200,
        .cut_fundamental   = 100,
        .cut_peak          = 235,
        .pitch_range       = 2.1202345,
        .cut_range         = 2.0249748,
};