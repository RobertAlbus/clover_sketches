#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <functional>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/pan.hpp"
#include "clover/dsp/wave.hpp"
using namespace clover;
using namespace dsp;

enum struct waveform {
    sine,
    saw,
    square,
    triangle,
    noise,
    none,
};

constexpr std::array<const char*, 6> waveform_str{
        "sine",
        "saw",
        "square",
        "triangle",
        "noise",
        "none",
};

const std::array<std::function<float(float)>, 6> waveform_func{
        clover::dsp::wave_sine,
        clover::dsp::wave_square,
        clover::dsp::wave_saw,
        clover::dsp::wave_tri,
        clover::dsp::wave_noise,
        [](float) { return 0; }};

waveform str_to_waveform(std::string_view str) noexcept;
const char* waveform_to_str(waveform wave) noexcept;
std::function<float(float)> waveform_to_func(waveform wave) noexcept;

struct nx_osc_props {
    float tuning;           // semitones.cents, relative
    float portamento_time;  // glide time in samples
    float pitch_env_octaves;

    std::vector<float> osc_tunings;  // semitones.cents, relative
    std::vector<float> osc_pans;     // [L,R] = [-1,1]
    std::vector<waveform> waveforms;
    int retrigger_i;

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
    std::string build_str_list_waveforms_i();
};

struct nx_osc {
    float fs;

    std::vector<oscillator> oscs;
    std::vector<pan> osc_pans;
    bool retrigger;
    nx_osc_props props;

    env_linear portamento;
    env_adsr adsr_pitch;
    env_adsr adsr_amp;

    nx_osc(float fs, const nx_osc_props& new_props);

    void note(float midi_note);
    void key_on();
    void key_off();
    void patch(nx_osc_props new_props);
    std::pair<float, float> tick();
};
