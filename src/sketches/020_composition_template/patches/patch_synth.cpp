// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/math.hpp"
#include "sequence/notes.h"

#include "patches.hpp"

patch_synth_t::patch_synth_t() {
    // --------------------------------
    // LEAD

    lead_a_props = {
            .osc_props{
                    .tuning            = -12,
                    .portamento_time   = 50,
                    .pitch_env_octaves = 2,
                    .osc_tunings       = {0, 0.2, -0.2, 0},
                    .osc_pans          = {0, -1.f, 1.f, 0},
                    .waveforms         = {waveform::saw, waveform::saw, waveform::saw, waveform::noise},
                    .retrigger         = true,
                    .pitch_a           = 200,
                    .pitch_d           = 800,
                    .pitch_s           = 0,
                    .pitch_r           = 100,
                    .amp_a             = 1000,
                    .amp_d             = 2000,
                    .amp_s             = 0.6,
                    .amp_r             = 1000,
            },
            .filter_props{
                    .cutoff               = 1500,
                    .cutoff_range_octaves = 3,
                    .res                  = 1,
                    .res_range_octaves    = 0,
                    .filter_type          = filter_t::lpf,
                    .cut_a                = 200,
                    .cut_d                = 800,
                    .cut_s                = 0.1,
                    .cut_r                = 100,
                    .res_a                = 10,
                    .res_d                = 1000,
                    .res_s                = 0,
                    .res_r                = 100,
            }};

    lead_b_props = {
            .tuning            = -24,
            .portamento_time   = 0,
            .pitch_env_octaves = 4,
            .osc_tunings       = {0, 5},
            .osc_pans          = {0, 0},
            .waveforms         = {waveform::saw, waveform::sine},
            .retrigger         = true,
            .pitch_a           = 1,
            .pitch_d           = 3000,
            .pitch_s           = 0,
            .pitch_r           = 100,
            .amp_a             = 1,
            .amp_d             = 2000,
            .amp_s             = 0.6,
            .amp_r             = 100,
    };

    lead_b_lfo_props = {
            .tuning            = -12,
            .portamento_time   = 1000,
            .pitch_env_octaves = 0.5,
            .osc_tunings       = {0},
            .osc_pans          = {0},
            .waveforms         = {waveform::saw},
            .retrigger         = true,
            .pitch_a           = 1,
            .pitch_d           = 500,
            .pitch_s           = 0,
            .pitch_r           = 100,
            .amp_a             = 1,
            .amp_d             = 1,
            .amp_s             = 1,
            .amp_r             = 100,
    };

    lead_peq_props = {};

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

    // --------------------------------
    // PAD

    pad_props = {
            .osc_props{
                    .tuning            = 0,
                    .portamento_time   = 0,
                    .pitch_env_octaves = 0,
                    .osc_tunings       = {0.1, -0.1, 0.2, -0.2, 0, 0},
                    .osc_pans          = {-1.f, 0.5f, -0.5f, 1.f, 1, -1},
                    // clang-format off
                    .waveforms = {waveform::saw, waveform::saw, waveform::square, waveform::square, waveform::noise, waveform::noise},
                    // clang-format on
                    .retrigger = true,
                    .pitch_a   = 1,
                    .pitch_d   = 1,
                    .pitch_s   = 0,
                    .pitch_r   = 1,
                    .amp_a     = 200000,
                    .amp_d     = 1,
                    .amp_s     = 1,
                    .amp_r     = 200000,
            },
            .filter_props{
                    .cutoff               = 500,
                    .cutoff_range_octaves = 3,
                    .res                  = 1,
                    .res_range_octaves    = 0,
                    .filter_type          = filter_t::lpf,
                    .cut_a                = 100000,
                    .cut_d                = 100000,
                    .cut_s                = 1,
                    .cut_r                = 100000,
                    .res_a                = 1,
                    .res_d                = 1,
                    .res_s                = 0,
                    .res_r                = 1,
            }};

    pad_preverb_peq_props = {
            peq_props{
                    .freq    = 18000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 18000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 273.4,
                    .reso    = 1,
                    .gain    = 15,
                    .enabled = true,
                    .type    = peq_filter_type::eq,
            },
            peq_props{
                    .freq    = 20,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };
    pad_fdn_props = {
            // clang-format off
                .taps = {
                    48000 / (midi_to_frequency(note::Fs3) / 8 / 2),
                    48000 / (midi_to_frequency(note::B3)  / 8 / 2),
                    48000 / (midi_to_frequency(note::Cs4) / 8 / 4),
                    48000 / (midi_to_frequency(note::E4)  / 8 / 4),
                    48000 / (midi_to_frequency(note::Gs4) / 8 / 8),
                    48000 / (midi_to_frequency(note::A4)  / 8 / 8),
                    48000 / (midi_to_frequency(note::As4) / 4 / 16),
                    48000 / (midi_to_frequency(note::Ds5) / 4 / 16)
                },
            // clang-format on
            .fb_gain = 0.89,
            .lpf_cut = 12875.011,
            .lpf_res = 0.707,
            .hpf_cut = 293,
            .hpf_res = 0.707,
    };
    pad_peq_props = {
            peq_props{
                    .freq    = 18000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 18000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 448.4,
                    .reso    = 0.2,
                    .gain    = 11.7,
                    .enabled = true,
                    .type    = peq_filter_type::eq,
            },
            peq_props{
                    .freq    = 100,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
    };
};