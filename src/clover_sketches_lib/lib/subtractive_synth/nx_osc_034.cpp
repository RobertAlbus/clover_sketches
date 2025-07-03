// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <format>
#include <ranges>
#include <string>
#include <utility>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"
#include "lib/audio_frame/audio_frame.hpp"

#include "nx_osc_034.hpp"

std::string build_str_list_floats(std::span<float> floats) {
    std::string result = "{";
    for (auto& value : floats)
        result += std::format("{}, ", std::to_string(value));
    result.pop_back();
    result += "}";
    return result;
}

// --------------------------------
// nx_osc_props

waveform_000 str_to_waveform(std::string_view str) noexcept {
    auto it = std::find(waveform_str_000.begin(), waveform_str_000.end(), str);
    return it != waveform_str_000.end() ? waveform_000::none
                                        : static_cast<waveform_000>(it - waveform_str_000.begin());
}

const char* waveform_to_str(waveform_000 wave) noexcept {
    return waveform_str_000[size_t(wave)];
}
std::function<float(float)> waveform_to_func(waveform_000 wave) noexcept {
    return waveform_func_000[size_t(wave)];
}

std::string nx_osc_props_034::to_str() {
    return std::format(
            "\
nx_osc_props patch = {{\n\
    .tuning            = {}, \n\
    .portamento_time   = {}, \n\
    .pitch_env_octaves = {}, \n\
    .osc_tunings       = {}, \n\
    .osc_pans          = {}, \n\
    .osc_gains         = {}, \n\
    .waveforms         = {}, \n\
    .retrigger         = {}, \n\
    .pitch_a           = {}, \n\
    .pitch_d           = {}, \n\
    .pitch_s           = {}, \n\
    .pitch_r           = {}, \n\
    .amp_a             = {}, \n\
    .amp_d             = {}, \n\
    .amp_s             = {}, \n\
    .amp_r             = {}, \n\
}};",
            tuning,
            portamento_time,
            pitch_env_octaves,
            build_str_list_floats(osc_tunings),
            build_str_list_floats(osc_pans),
            build_str_list_floats(osc_gains),
            build_str_list_waveforms_i(),
            retrigger,
            pitch_a,
            pitch_d,
            pitch_s,
            pitch_r,
            amp_a,
            amp_d,
            amp_s,
            amp_r);
}

std::string nx_osc_props_034::build_str_list_waveforms_i() {
    std::string result = "{";
    for (auto& waveform : waveforms)
        result += std::format("waveform_000::{}, ", waveform_to_str(waveform));
    result.pop_back();
    result += "}";
    return result;
}

// --------------------------------
// nx_osc

nx_osc_034::nx_osc_034(float fs, const nx_osc_props_034& new_props) : fs{fs} {
    patch(new_props);
}

void nx_osc_034::note(float midi_note) {
    portamento.set(midi_note, props.portamento_time);
}

void nx_osc_034::key_on() {
    adsr_pitch.key_on();
    adsr_amp.key_on();
    if (retrigger)
        for (auto& osc : oscs)
            osc.phase(0);
}

void nx_osc_034::key_off() {
    adsr_pitch.key_off();
    adsr_amp.key_off();
}

void nx_osc_034::patch(nx_osc_props_034 new_props) {
    props = std::move(new_props);

    for (auto waveform_i : props.waveforms) {
        osc_pans.emplace_back();
    }
    for (auto [osc_pan, osc_pan_value] : std::views::zip(osc_pans, props.osc_pans)) {
        osc_pan.set(osc_pan_value);
    }
    for (auto waveform : props.waveforms) {
        auto& osc    = oscs.emplace_back(fs);
        osc.waveform = waveform_to_func(waveform);
    }

    gain_scale = 1.f / std::sqrt(float(oscs.size()));
    retrigger  = props.retrigger;

    adsr_pitch.set(props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);
    adsr_amp.set(props.amp_a, props.amp_d, props.amp_s, props.amp_r);
}

audio_frame nx_osc_034::tick() {
    float current_midi_note = portamento.tick() + props.tuning;
    float pitch_env         = adsr_pitch.tick() * props.pitch_env_octaves;

    audio_frame osc_output = 0;

    for (auto [osc, osc_gain, osc_tuning, osc_pan] :
         std::views::zip(oscs, props.osc_gains, props.osc_tunings, osc_pans)) {
        float osc_freq;
        osc_freq = clover::midi_to_frequency(current_midi_note + osc_tuning);
        osc_freq = clover::frequency_by_octave_difference(osc_freq, pitch_env);
        osc_freq = clover::frequency_by_octave_difference(osc_freq, input_mod_pitch_octaves);
        osc_freq = std::clamp(osc_freq, 0.f, fs * 0.499f);

        osc.freq(osc_freq);

        audio_frame osc_signal = osc.tick() * osc_gain;
        osc_output += osc_pan.tick(osc_signal);
    }

    osc_output *= gain_scale;

    float amp_env = adsr_amp.tick();
    osc_output *= amp_env;

    return osc_output;
}
