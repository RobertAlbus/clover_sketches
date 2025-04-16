// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/math.hpp"

#include "patches.hpp"
#include "sequence/notes.h"

patch_drums_t::patch_drums_t() {
    // --------------------------------
    // KICK
    kick_drum_props = {
            .trim              = 0.84297022,
            .drive             = 2.6860298,
            .amp_a             = 30,
            .amp_d             = 5000,
            .amp_s             = 0.25475893,
            .amp_r             = 1000,
            .cut_a             = 1,
            .cut_d             = 5809.8223,
            .cut_s             = 0,
            .cut_r             = 1.0000596,
            .filt_q            = 1.3999405,
            .pitch_a           = 1,
            .pitch_d           = 2999.8784,
            .pitch_s           = 0,
            .pitch_r           = 1000,
            .pitch_fundamental = midi_to_frequency(note::F2) / 2,
            .cut_fundamental   = 100,
            .pitch_range       = 2,
            .cut_range         = 3,
    };

    kick_fdn_props = {
            .taps    = {134.42862, 329.99323, 721.1825, 1308.0359, 1959.9309, 3329.2222, 6458.37, 11087.26},
            .fb_gain = 0.87402976,
            .lpf_cut = 180.31549,
            .lpf_res = 0.707,
            .hpf_cut = 45.100502,
            .hpf_res = 0.707,
    };

    kick_preverb_peq_props = {
            peq_props{
                    .freq    = 20,
                    .reso    = .707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = .707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };

    kick_peq_props = {
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 41,
                    .reso    = 1.1,
                    .gain    = -2.0,
                    .enabled = true,
                    .type    = peq_filter_type::ls,
            },
            peq_props{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };

    // --------------------------------
    // BASS
    bass_props = {
            .osc_props =
                    {
                            .tuning            = -24,
                            .portamento_time   = 0,
                            .pitch_env_octaves = 0,
                            .osc_tunings       = {0, 12.10f, 11.90f},
                            .osc_pans          = {0, -0.5f, 0.5f},
                            .waveforms         = {waveform::saw, waveform::saw, waveform::saw},
                            .retrigger         = true,
                            .pitch_a           = 20,
                            .pitch_d           = 1000,
                            .pitch_s           = 0,
                            .pitch_r           = 100,
                            .amp_a             = 100,
                            .amp_d             = 3000,
                            .amp_s             = 0.1,
                            .amp_r             = 2000,
                    },
            .filter_props = {
                    .cutoff               = 100,
                    .cutoff_range_octaves = 4,
                    .res                  = 0.907,
                    .res_range_octaves    = 0,
                    .filter_type          = filter_t::lpf,
                    .cut_a                = 100,
                    .cut_d                = 2000,
                    .cut_s                = 0.3,
                    .cut_r                = 1000,
                    .res_a                = 1000,
                    .res_d                = 1000,
                    .res_s                = 0.1,
                    .res_r                = 1000,
            }};
    bass_peq_props = {
            peq_props{
                    .freq    = 1921.8,
                    .reso    = 2.8,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 3216.8,
                    .reso    = 0.1,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 60,
                    .reso    = 0.707,
                    .gain    = -4,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 60,
                    .reso    = 0.707,
                    .gain    = -13.8,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };

    // --------------------------------
    // CYMBALS

    hh1_props = {
            .freqs            = {31, 82, 215, 227, 248, 278},
            .hpf_f0           = 1992,
            .hpf_fmod_octaves = 2.3,
            .hpf_Q            = 2.11,

            .bpf_f0 = 7984,
            .bpf_Q  = 1.4,

            .amp_a = 10,
            .amp_d = 1500,
            .amp_s = 0,
            .amp_r = 10,

            .cut_a = 10,
            .cut_d = 500,
            .cut_s = 0,
            .cut_r = 100,
    };
    hh2_props = {
            .freqs            = {100, 150, 175, 225, 66.6},
            .hpf_f0           = 100,
            .hpf_fmod_octaves = 5,
            .hpf_Q            = 1,

            .bpf_f0 = 3000,
            .bpf_Q  = 1.3,

            .amp_a = 10,
            .amp_d = 1500,
            .amp_s = 0,
            .amp_r = 10,

            .cut_a = 10,
            .cut_d = 800,
            .cut_s = 0,
            .cut_r = 100,
    };
    hh3_props = {
            .osc_props =
                    {
                            .tuning            = 0,
                            .portamento_time   = 0,
                            .pitch_env_octaves = 0,
                            .osc_tunings       = {0},
                            .osc_pans          = {-0.1f, -0.4f},
                            .waveforms         = {waveform::noise, waveform::noise},
                            .retrigger         = false,
                            .pitch_a           = 10,
                            .pitch_d           = 10,
                            .pitch_s           = 0,
                            .pitch_r           = 10,
                            .amp_a             = 10,
                            .amp_d             = 2000,
                            .amp_s             = 0,
                            .amp_r             = 1000,
                    },
            .filter_props = {
                    .cutoff               = 2000,
                    .cutoff_range_octaves = 2,
                    .res                  = 1,
                    .res_range_octaves    = 1,
                    .filter_type          = filter_t::bpf,
                    .cut_a                = 10,
                    .cut_d                = 1500,
                    .cut_s                = 0,
                    .cut_r                = 100,
                    .res_a                = 10,
                    .res_d                = 1000,
                    .res_s                = 0,
                    .res_r                = 100,
            }};
    hh_fdn_props = {
            .taps    = {134.391, 293.781, 721.152, 786.348, 395.174, 721.152, 264.783, 69.196},
            .fb_gain = 0.612,
            .lpf_cut = 13899.115,
            .lpf_res = 0.707,
            .hpf_cut = 2719.1443,
            .hpf_res = 0.707,
    };
    hh_preverb_peq_props = {
            peq_props{
                    .freq    = 11020.2,
                    .reso    = 3.5,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 5219.8,
                    .reso    = 2,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };
    hh_peq_props = {
            peq_props{
                    .freq    = 5243.7,
                    .reso    = 3.9,
                    .gain    = -7.1,
                    .enabled = true,
                    .type    = peq_filter_type::eq,
            },
            peq_props{
                    .freq    = 16083.7,
                    .reso    = 0.8,
                    .gain    = -0.8,
                    .enabled = false,
                    .type    = peq_filter_type::hs,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };
    ride_props = {
            .freqs            = {150, 404, 1533, 1751, 13456, 17500},
            .hpf_f0           = 100,
            .hpf_fmod_octaves = 6,
            .hpf_Q            = 1,

            .bpf_f0 = 3000,
            .bpf_Q  = 1.3,

            .amp_a = 400,
            .amp_d = 1,
            .amp_s = 1,
            .amp_r = 15000,

            .cut_a = 30,
            .cut_d = 1,
            .cut_s = 1,
            .cut_r = 2500000,
    };
    ride_peq_props = {
            peq_props{
                    .freq    = 5938.7,
                    .reso    = 0.9,
                    .gain    = 14,
                    .enabled = true,
                    .type    = peq_filter_type::eq,
            },
            peq_props{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };
}
