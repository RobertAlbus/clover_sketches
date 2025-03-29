#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/math.hpp"
#include "instruments/cymbal.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/notes.h"

struct patch_drums_t {
    // --------------------------------
    // KICK

    struct kick_mix_t {
        float send = 0.8f;
        float wet  = 1.f;
        float gain = 1.f;
    };

    kick_mix_t kick_mix;

    kick_props kick_drum_props{
            .trim              = 0.8429702,
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
    fdn_8_props_012 kick_fdn_props{
            .taps    = {134.42862, 329.99323, 721.1825, 1308.0359, 1959.9309, 3329.2222, 6458.37, 11087.26},
            .fb_gain = 0.87402976,
            .lpf_cut = 180.31549,
            .lpf_res = 0.707,
            .hpf_cut = 45.100502,
            .hpf_res = 0.707,
    };

    std::array<peq_props, peq::SIZE> kick_preverb_peq_props{};
    std::array<peq_props, peq::SIZE> kick_peq_props{};

    // --------------------------------
    // BASS

    subtractive_synth_props bass_props{
            .osc_props =
                    {
                            .tuning            = -24,
                            .portamento_time   = 0,
                            .pitch_env_octaves = 0,
                            .osc_tunings       = {0, 12.10f, 11.90f},
                            .osc_pans          = {0, -0.5f, 0.4f},
                            .waveforms         = {waveform::saw, waveform::saw, waveform::saw},
                            .retrigger_i       = 1,
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
    std::array<peq_props, peq::SIZE> bass_peq_props{};

    struct bass_mix_t {
        float gain = 0.5f;
    };

    bass_mix_t bass_mix;

    // --------------------------------
    // CYMBALS

    cymbal_props hh1_props{
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

    cymbal_props hh2_props{
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

    // cymbal_props hh2_props{
    //         .freqs            = {200, 250, 275, 325, 166.6},
    //         .hpf_f0           = 100,
    //         .hpf_fmod_octaves = 5,
    //         .hpf_Q            = 1,

    //         .bpf_f0 = 3000,
    //         .bpf_Q  = 1.3,

    //         .amp_a = 10,
    //         .amp_d = 2500,
    //         .amp_s = 0,
    //         .amp_r = 10,

    //         .cut_a = 10,
    //         .cut_d = 1800,
    //         .cut_s = 0,
    //         .cut_r = 100,
    // };
    subtractive_synth_props hh3_props{
            .osc_props =
                    {
                            .tuning            = 0,
                            .portamento_time   = 0,
                            .pitch_env_octaves = 0,
                            .osc_tunings       = {0, 33.10f, 33.90f},
                            .osc_pans          = {0, -0.5f, 0.2f},
                            .waveforms         = {waveform::noise, waveform::saw, waveform::saw},
                            .retrigger_i       = 0,
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
                    .cutoff               = 8000,
                    .cutoff_range_octaves = 4,
                    .res                  = 1,
                    .res_range_octaves    = 1,
                    .filter_type          = filter_t::bpf,
                    .cut_a                = 100,
                    .cut_d                = 3000,
                    .cut_s                = 0.3,
                    .cut_r                = 100,
                    .res_a                = 100,
                    .res_d                = 3000,
                    .res_s                = 0,
                    .res_r                = 100,
            }};

    fdn_8_props_012 hh_fdn_props{
            .taps    = {13.442862, 32.999323, 72.11825, 130.80359, 195.99309, 332.92222, 645.837, 1108.726},
            .fb_gain = 0.87402976,
            .lpf_cut = 1800.31549,
            .lpf_res = 0.707,
            .hpf_cut = 450.100502,
            .hpf_res = 0.707,
    };
    std::array<peq_props, peq::SIZE> hh_preverb_peq_props{};
    std::array<peq_props, peq::SIZE> hh_peq_props{};

    cymbal_props ride_props{};
    std::array<peq_props, peq::SIZE> ride_peq_props{};

    struct hh_mix_t {
        float mix_hh1 = 1.f;
        float mix_hh2 = 0.6f;
        float mix_hh3 = 1.f;

        float hh_verb_send = 2.f;
        float hh_verb_wet  = .2f;
        float mix_hh       = 1.f;

        float mix_ride = 1.f;

        float mix_drive = 1.8f;
        float mix_trim  = 0.808f;
        float mix_bus   = 0.7f;
    };
    hh_mix_t hh_mix;
};
