#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/oscillator.hpp"

#include "lib/audio_frame/audio_frame.hpp"
#include "lib/mixer/pan_033.hpp"
#include "lib/subtractive_synth/nx_osc_000.hpp"

template <typename prop_type>
struct nx_osc_props_034_t {
    prop_type tuning;           // semitones.cents, relative
    prop_type portamento_time;  // glide time in samples
    prop_type pitch_env_octaves;
    bool retrigger;

    prop_type amp_a;
    prop_type amp_d;
    prop_type amp_s;
    prop_type amp_r;
    prop_type pitch_a;
    prop_type pitch_d;
    prop_type pitch_s;
    prop_type pitch_r;

    std::vector<prop_type> osc_tunings;  // semitones.cents, relative
    std::vector<prop_type> osc_pans;     // [L,R] = [-1,1]
    std::vector<prop_type> osc_gains;
    std::vector<waveform_000> waveforms;
};

using nx_osc_props_034 = nx_osc_props_034_t<float>;

std::string to_str(const nx_osc_props_034& osc_props);

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
    void initialize(size_t size);
    void patch(nx_osc_props_034 new_props);
    audio_frame tick();
};