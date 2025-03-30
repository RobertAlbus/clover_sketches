#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "instruments/fdn.hpp"
#include "instruments/nx_osc.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"

struct patch_synth_t {
    // --------------------------------
    // LEAD

    subtractive_synth_props lead_props{
            .osc_props{
                    .tuning            = 0,
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

    nx_osc_props lead_ringmod_props{};
    std::array<peq_props, peq::SIZE> lead_peq_props{};

    // --------------------------------
    // CHORD

    subtractive_synth_props chord_props{
            .osc_props{
                    .tuning            = 0,
                    .portamento_time   = 0,
                    .pitch_env_octaves = 0,
                    .osc_tunings       = {0.1, -0.1},
                    .osc_pans          = {-0.1f, 0.5f},
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

    fdn_8_props_019 chord_fdn_props{
            .taps    = {134.42862, 329.99323, 721.1825, 1308.0359, 1959.9309, 3329.2222, 6458.37, 11087.26},
            .fb_gain = 0.92402976,
            .lpf_cut = 14000.31549,
            .lpf_res = 0.707,
            .hpf_cut = 45.100502,
            .hpf_res = 0.707,
    };
    std::array<peq_props, peq::SIZE> chord_preverb_peq_props{};
    std::array<peq_props, peq::SIZE> chord_peq_props{};

    // --------------------------------
    // PAD

    const float ppw = 0.3; // pad pitch width
    subtractive_synth_props pad_props{
            .osc_props{
                    // clang-format off
                    .tuning            = 12,
                    .portamento_time   = 0,
                    .pitch_env_octaves = 0,
                    .osc_tunings       = {ppw, -ppw, 12 + ppw, 12 - ppw},
                    .osc_pans = {-1.f, 1.f, -1.f, 1.f},
                    .waveforms = {waveform::saw, waveform::saw, waveform::saw, waveform::saw},
                    .retrigger = true,
                    .pitch_a   = 1,
                    .pitch_d   = 1,
                    .pitch_s   = 0,
                    .pitch_r   = 1,
                    .amp_a     = 200000,
                    .amp_d     = 1,
                    .amp_s     = 1,
                    .amp_r     = 200000,
                    // clang-format on
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

    std::array<peq_props, peq::SIZE> pad_preverb_peq_props{};
    alignas(64) fdn_8_props_019 pad_fdn_props{
            .taps    = {134.42862, 329.99323, 721.1825, 1308.0359, 1959.9309, 3329.2222, 6458.37, 11087.26},
            .fb_gain = 0.92402976,
            .lpf_cut = 14000.31549,
            .lpf_res = 0.707,
            .hpf_cut = 45.100502,
            .hpf_res = 0.707,
    };
    std::array<peq_props, peq::SIZE> pad_peq_props{};
};