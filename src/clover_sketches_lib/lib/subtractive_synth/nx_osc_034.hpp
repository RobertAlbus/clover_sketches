#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <portaudio.h>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/oscillator.hpp"

#include "lib/audio_frame/audio_frame.hpp"
#include "lib/mixer/pan_033.hpp"
#include "lib/subtractive_synth/nx_osc_000.hpp"

struct nx_osc_props_034 {
    float tuning;           // semitones.cents, relative
    float portamento_time;  // glide time in samples
    float pitch_env_octaves;

    std::vector<float> osc_tunings;  // semitones.cents, relative
    std::vector<float> osc_pans;     // [L,R] = [-1,1]
    std::vector<float> osc_gains;
    std::vector<waveform_000> waveforms;
    bool retrigger;

    float pitch_a;
    float pitch_d;
    float pitch_s;
    float pitch_r;
    float amp_a;
    float amp_d;
    float amp_s;
    float amp_r;

    std::string to_str();
    std::string build_str_list_osc_tunings();
    std::string build_str_list_osc_pans();
    std::string build_str_list_osc_gains();
    std::string build_str_list_waveforms_i();
};

struct nx_osc_034 {
    float fs;
    float gain_scale = 1.f;

    std::vector<clover::dsp::oscillator> oscs;
    std::vector<pan_033> osc_pans;
    bool retrigger;
    nx_osc_props_034 props;

    float input_mod_pitch_octaves = 0;
    clover::dsp::env_linear portamento;
    clover::dsp::env_adsr adsr_pitch;
    clover::dsp::env_adsr adsr_amp;

    nx_osc_034(float fs, const nx_osc_props_034& new_props);

    void note(float midi_note);
    void key_on();
    void key_off();
    void patch(nx_osc_props_034 new_props);
    audio_frame tick();
};
