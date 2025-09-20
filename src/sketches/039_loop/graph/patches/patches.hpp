#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "graph/instrument/driver.hpp"
#include "graph/instrument/ducker.hpp"
#include "lib/cymbal/cymbal_038.hpp"
#include "lib/fdn/fdn8_023.hpp"
#include "lib/fdn/fdn8_stereo_030.hpp"
#include "lib/fm/fm_037.hpp"
#include "lib/kick_drum/kick_drum_038.hpp"
#include "lib/mixer/mixer_033.hpp"
#include "lib/peq/peq.hpp"
#include "lib/subtractive_synth/subtractive_synth_038.hpp"

struct patch_automation_t {
    patch_automation_t(float fs, float bpm);

    // --------------------------------
    // SIDECHAIN

    ducker_props sc_pump_props;
};

struct patch_drums_t {
    patch_drums_t();

    // --------------------------------
    // KICK

    kick_props_038 kick_drum_props;
    fdn8_props_023 kick_fdn_props;
    std::array<peq_props_000, peq_000::SIZE> kick_preverb_peq_props;
    std::array<peq_props_000, peq_000::SIZE> kick_postverb_peq_props;
    std::array<peq_props_000, peq_000::SIZE> kick_peq_props;

    // --------------------------------
    // SNARE

    cymbal_props_038 snare_body_impulse_props;
    fdn8_stereo_030_props snare_body_resonator_props;
    driver::properties snare_body_driver_props;
    std::array<peq_props_000, peq_000::SIZE> snare_body_peq_props;

    // --------------------------------
    // RIDE

    std::array<peq_props_000, peq_000::SIZE> ride_peq_props;
    cymbal_props_038 ride_props;

    // --------------------------------
    // HH

    cymbal_props_038 hh1_props;
    driver::properties hh1_driver_props;
    std::array<peq_props_000, peq_000::SIZE> hh1_peq_props;

    cymbal_props_038 hh2_props;
    driver::properties hh2_driver_props;
    std::array<peq_props_000, peq_000::SIZE> hh2_peq_props;

    cymbal_props_038 hh3_props;
    std::array<peq_props_000, peq_000::SIZE> hh3_peq_props;
};

struct patch_synth_t {
    patch_synth_t();

    // --------------------------------
    // BASS
    fm_props_037 bass_fm_props;
    fdn8_stereo_030_props bass_fdn_props;
    std::array<peq_props_000, peq_000::SIZE> bass_preverb_peq_props;
    std::array<peq_props_000, peq_000::SIZE> bass_postverb_peq_props;
    std::array<peq_props_000, peq_000::SIZE> bass_peq_props;

    // --------------------------------
    // CHORD

    subtractive_synth_props_038 chord_1_props;
    fdn8_stereo_030_props chord_1_fdn_props;
    std::array<peq_props_000, peq_000::SIZE> chord_1_peq_props;

    subtractive_synth_props_038 chord_2_props;
    fdn8_stereo_030_props chord_2_fdn_props;
    std::array<peq_props_000, peq_000::SIZE> chord_2_peq_props;
};

struct patch_mix_t {
    patch_mix_t();

    // std::vector<mixer_track_000> mixer_tracks;

    mixer_033::list_specs new_mixer_spec;

    // --------------------------------
    // MASTER

    std::array<peq_props_000, peq_000::SIZE> main_peq_props;
};

struct patch {
    patch(float fs, float bpm) : automation{fs, bpm} {
    }
    patch_automation_t automation;
    patch_drums_t drums;
    patch_synth_t synth;
    patch_mix_t mix;
};

/*
high reso eq boost (with cut mod) into this "kick" chain is mega heady and tribal

{
    .taps     = {617.7716, 687.3982, 797.0081, 17977.783, 22838.275, 259.32178, 60.076298, 54.269703},
    .fb_gain  = 0.95,
    .lpf_cut  = 1290.2051,
    .lpf_res  = 0.707,
    .hpf_cut  = 54.57113,
    .hpf_res  = 0.707,
};

{
            .trim              = 0.976,
            .drive             = 7.024,
            .amp_a             = 30,
            .amp_d             = 477.17914,
            .amp_s             = 0.23809522,
            .amp_r             = 298.3214,
            .cut_a             = 1,
            .cut_d             = 1524.3208,
            .cut_s             = 0,
            .cut_r             = 1.0000596,
            .filt_q            = 1.3999405,
            .pitch_a           = 1,
            .pitch_d           = 1690.4856,
            .pitch_s           = 0,
            .pitch_r           = 1000,
            .pitch_fundamental = 43.653526,
            .cut_fundamental   = 100,
            .pitch_range       = 1,
            .cut_range         = 4,
        };

PULSING CHORDS BIG KICK

kick
{
            .trim              = 0.976,
            .drive             = 7.262,
            .amp_a             = 30,
            .amp_d             = 12857.822,
            .amp_s             = 0,
            .amp_r             = 262.64288,
            .cut_a             = 1,
            .cut_d             = 1191.4169,
            .cut_s             = 0,
            .cut_r             = 1.0000596,
            .filt_q            = 1.3999405,
            .pitch_a           = 1,
            .pitch_d           = 2143.643,
            .pitch_s           = 0,
            .pitch_r           = 226.96426,
            .pitch_fundamental = 43.653526,
            .cut_fundamental   = 100,
            .pitch_range       = 1,
            .cut_range         = 3,
        };

        kick verb
{
    .taps     = {62.60503, 68.16708, 78.93622, 111.50018, 156.06538, 218.93523, 376.53464, 1683.6116},
    .fb_gain  = 0.981,
    .lpf_cut  = 978.7971,
    .lpf_res  = 0.707,
    .hpf_cut  = 94.818924,
    .hpf_res  = 0.707,
};

chord verb
{
    .taps     = {164.07095, 421.93472, 1540.1703, 1600.0818, 1540.1703, 22919.785, 22343.027, 3754.2483},
    .fb_gain  = 0.857,
    .lpf_cut  = 9744.787,
    .lpf_res  = 0.707,
    .hpf_cut  = 69.70979,
    .hpf_res  = 0.707,
};


god tier late reflection on chords
{
    .taps     = {18455.613, 640.97437, 765.5951, 974.5385, 2873.3826, 6328.767, 2985.2551, 6744.995},
    .fb_gain  = 0.899,
    .lpf_cut  = 978.7971,
    .lpf_res  = 0.707,
    .hpf_cut  = 143.76382,
    .hpf_res  = 0.707,
};



*/