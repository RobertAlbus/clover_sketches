// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/patches/deep.hpp"

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