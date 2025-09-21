#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"

#include "lib/_atom/adsr/adsr.hpp"
#include "lib/_atom/filter_type.hpp"
#include "lib/audio_frame/audio_frame.hpp"
#include "lib/mixer/pan_033.hpp"
#include "lib/subtractive_synth/nx_osc_000.hpp"

struct subtractive_synth_props_038 {
    static const int NUM_OSCS = 6;

    float tuning            = 0;
    float portamento_time   = 0;
    float pitch_env_octaves = 0;
    bool retrigger          = true;

    std::array<float, NUM_OSCS> osc_tunings      = {0, 0, 0, 0, 0, 0};
    std::array<float, NUM_OSCS> osc_pans         = {0, 0, 0, 0, 0, 0};
    std::array<float, NUM_OSCS> osc_gains        = {1, 0, 0, 0, 0, 0};
    std::array<waveform_000, NUM_OSCS> waveforms = {
        waveform_000::sine,
        waveform_000::sine,
        waveform_000::sine,
        waveform_000::sine,
        waveform_000::sine,
        waveform_000::sine};

    adsr_values pitch_adsr = {1000, 1000, 1, 1000};
    adsr_values amp_adsr   = {1000, 1000, 1, 1000};
    adsr_values cut_adsr   = {1000, 1000, 1, 1000};
    adsr_values res_adsr   = {1000, 1000, 1, 1000};

    float cut               = 1000;
    float res               = 1;
    float cut_env_octaves   = 0;
    float res_env_octaves   = 0;
    filter_type filter_type = filter_type::lp;

    // WARNING: heap allocation
    std::string to_str();
};

struct subtractive_synth_038 {
    static const int NUM_OSCS = 6;

    float fs;
    subtractive_synth_props_038 props;

    std::array<clover::dsp::oscillator, NUM_OSCS> oscs;
    std::array<pan_033, NUM_OSCS> osc_pans;
    clover::dsp::env_linear portamento;
    clover::dsp::env_adsr adsr_pitch;
    clover::dsp::env_adsr adsr_amp;

    clover::dsp::filter_2 filter;
    clover::dsp::env_adsr adsr_cut;
    clover::dsp::env_adsr adsr_res;

    float input_mod_pitch_octaves = 0;

    subtractive_synth_038(float fs, const subtractive_synth_props_038& new_props);

    void note(float midi_note);
    void key_on(float midi_note);
    void key_off();
    void patch(subtractive_synth_props_038 new_props);
    audio_frame tick();
};
