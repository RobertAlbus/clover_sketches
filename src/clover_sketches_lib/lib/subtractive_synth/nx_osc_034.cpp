// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <format>
#include <ranges>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"
#include "lib/audio_frame/audio_frame.hpp"
#include "lib/subtractive_synth/nx_osc_000.hpp"

#include "nx_osc_034.hpp"

std::string build_str_list_floats(std::span<const float> floats) {
    std::string result = "{";
    for (auto& value : floats)
        result += std::format("{}, ", std::to_string(value));
    result.pop_back();
    result += "}";
    return result;
}
std::string build_str_list_waveforms(std::span<const waveform_000> waveforms) {
    std::string result = "{";
    for (auto& waveform : waveforms)
        result += std::format("waveform_000::{}, ", waveform_to_str(waveform));
    result.pop_back();
    result += "}";
    return result;
}

// --------------------------------
// nx_osc_props

std::string to_str(const nx_osc_props_034& osc_props) {
    return std::format(
        "\
{{\n\
    .tuning            = {}, \n\
    .portamento_time   = {}, \n\
    .pitch_env_octaves = {}, \n\
    .retrigger         = {}, \n\
    .amp_a             = {}, \n\
    .amp_d             = {}, \n\
    .amp_s             = {}, \n\
    .amp_r             = {}, \n\
    .pitch_a           = {}, \n\
    .pitch_d           = {}, \n\
    .pitch_s           = {}, \n\
    .pitch_r           = {}, \n\
    .osc_tunings       = {}, \n\
    .osc_pans          = {}, \n\
    .osc_gains         = {}, \n\
    .waveforms         = {}, \n\
    }};",
        osc_props.tuning,
        osc_props.portamento_time,
        osc_props.pitch_env_octaves,
        osc_props.retrigger,
        osc_props.pitch_a,
        osc_props.pitch_d,
        osc_props.pitch_s,
        osc_props.pitch_r,
        osc_props.amp_a,
        osc_props.amp_d,
        osc_props.amp_s,
        osc_props.amp_r,
        build_str_list_floats(osc_props.osc_tunings),
        build_str_list_floats(osc_props.osc_pans),
        build_str_list_floats(osc_props.osc_gains),
        build_str_list_waveforms(osc_props.waveforms));
}

// --------------------------------
// nx_osc

nx_osc_034::nx_osc_034(float fs, const nx_osc_props_034& new_props) : fs{fs} {
    initialize(new_props.waveforms.size());
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

void nx_osc_034::initialize(size_t size) {
    osc_pans = {};
    oscs     = {};
    for (auto _ : std::views::iota(0u, size)) {
        osc_pans.emplace_back();
        oscs.emplace_back(fs);
    }
}
void nx_osc_034::patch(nx_osc_props_034 new_props) {
    props = new_props;

    gain_scale = 1.f / std::sqrt(float(oscs.size()));

    for (auto [osc_pan, osc_pan_value] : std::views::zip(osc_pans, props.osc_pans)) {
        osc_pan.set(osc_pan_value);
    }
    for (auto [osc, waveform] : std::views::zip(oscs, props.waveforms)) {
        osc.waveform = waveform_to_func(waveform);
    }

    retrigger = props.retrigger;

    adsr_pitch.set(props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);
    adsr_amp.set(props.amp_a, props.amp_d, props.amp_s, props.amp_r);
}

audio_frame nx_osc_034::tick() {
    float current_midi_note = portamento.tick() + props.tuning;
    float pitch_env         = (adsr_pitch.tick() * props.pitch_env_octaves) + input_mod_pitch_octaves;

    audio_frame osc_output = 0;

    auto osc_zip = std::views::zip(oscs, osc_pans, props.osc_gains, props.osc_tunings);
    for (auto [osc, osc_pan, osc_gain, osc_tuning] : osc_zip) {
        float osc_freq = 0;
        osc_freq       = clover::midi_to_frequency(current_midi_note + osc_tuning);
        osc_freq       = clover::frequency_by_octave_difference(osc_freq, pitch_env);
        osc_freq       = std::clamp(osc_freq, 0.f, fs * 0.499f);

        osc.freq(osc_freq);

        audio_frame osc_signal = osc.tick() * osc_gain;
        osc_output += osc_pan.tick(osc_signal);
    }

    osc_output *= gain_scale;

    float amp_env = adsr_amp.tick();
    osc_output *= amp_env;

    return osc_output;
}
