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

#include "composition/settable.hpp"

enum struct waveform {
    sine,
    saw,
    square,
    triangle,
    noise,
    none,
};

constexpr std::array<const char*, 6> waveform_str {
    "sine",
    "saw",
    "square",
    "triangle",
    "noise",
    "none",
};

float wave_none(float) {return 0;}
const std::array<std::function<float(float)>, 6> waveform_func {
    clover::dsp::wave_sine,
    clover::dsp::wave_square,
    clover::dsp::wave_saw,
    clover::dsp::wave_tri,
    clover::dsp::wave_noise,
    wave_none
};

waveform str_to_waveform(std::string_view str) noexcept {
    auto it = std::find(waveform_str.begin(), waveform_str.end(), str);
    return it != waveform_str.end() ? waveform::none : static_cast<waveform>(it - waveform_str.begin());
}

const char* waveform_to_str(waveform wave) noexcept{
    return waveform_str[size_t(wave)];
}
std::function<float(float)> waveform_to_func(waveform wave) noexcept{
    return waveform_func[size_t(wave)];
}

struct nx_osc_props {
    settable tuning;             // semitones.cents, relative
    settable portamento_time;    // glide time in samples
    settable pitch_env_octaves;

    std::vector<settable> osc_tunings;  // semitones.cents, relative
    std::vector<settable> osc_pans; // [L,R] = [-1,1]
    std::vector<settable_int> waveforms_i;

    settable pitch_a;
    settable pitch_d;
    settable pitch_s;
    settable pitch_r;
    settable amp_a;
    settable amp_d;
    settable amp_s;
    settable amp_r;

    std::string to_str();
    std::string build_str_list_osc_tunings();
    std::string build_str_list_osc_pans();
    std::string build_str_list_waveforms_i();
    void tick();
};

struct nx_osc{
    float fs;

    std::vector<oscillator> oscs;
    std::vector<pan> osc_pans;
    nx_osc_props props;

    env_linear portamento;
    env_adsr adsr_pitch;
    env_adsr adsr_amp;

    void note(float midi_note);
    void key_on();
    void key_off();
    void patch(const nx_osc_props& new_props);
    void update_from_props();
    std::pair<float, float> tick();
};
