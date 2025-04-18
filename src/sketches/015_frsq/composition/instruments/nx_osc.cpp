// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <ranges>
#include <string>
#include <utility>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/pan.hpp"
#include "clover/math.hpp"
using namespace clover;
using namespace dsp;

#include "nx_osc.hpp"

// --------------------------------
// nx_osc_props

waveform str_to_waveform(std::string_view str) noexcept {
    auto it = std::find(waveform_str.begin(), waveform_str.end(), str);
    return it != waveform_str.end() ? waveform::none : static_cast<waveform>(it - waveform_str.begin());
}

const char* waveform_to_str(waveform wave) noexcept {
    return waveform_str[size_t(wave)];
}
std::function<float(float)> waveform_to_func(waveform wave) noexcept {
    return waveform_func[size_t(wave)];
}

void nx_osc_props::tick() {
    tuning.tick();
    portamento_time.tick();
    pitch_env_octaves.tick();

    for (auto [osc_tuning, osc_pan] : std::views::zip(osc_tunings, osc_pans)) {
        osc_tuning.tick();
        osc_pan.tick();
    }

    pitch_a.tick();
    pitch_d.tick();
    pitch_s.tick();
    pitch_r.tick();
    amp_a.tick();
    amp_d.tick();
    amp_s.tick();
    amp_r.tick();
}

std::string nx_osc_props::to_str() {
    return std::format(
            "\
nx_osc_props patch = {{\n\
    .tuning            = {}, \n\
    .portamento_time   = {}, \n\
    .pitch_env_octaves = {}, \n\
    .osc_tunings       = {}, \n\
    .osc_pans          = {}, \n\
    .waveforms_i       = {}, \n\
    .retrigger_i       = {}, \n\
    .pitch_a           = {}, \n\
    .pitch_d           = {}, \n\
    .pitch_s           = {}, \n\
    .pitch_r           = {}, \n\
    .amp_a             = {}, \n\
    .amp_d             = {}, \n\
    .amp_s             = {}, \n\
    .amp_r             = {}, \n\
}};",
            tuning.load_output(),
            portamento_time.load_output(),
            pitch_env_octaves.load_output(),
            build_str_list_osc_tunings(),
            build_str_list_osc_pans(),
            build_str_list_waveforms_i(),
            retrigger_i.load_output(),
            pitch_a.load_output(),
            pitch_d.load_output(),
            pitch_s.load_output(),
            pitch_r.load_output(),
            amp_a.load_output(),
            amp_d.load_output(),
            amp_s.load_output(),
            amp_r.load_output());
}
std::string nx_osc_props::build_str_list_osc_tunings() {
    std::string result = "{";
    for (auto& osc_tuning : osc_tunings)
        result += std::format("{}, ", std::to_string(osc_tuning.load_output()));
    result.pop_back();
    result += "}";
    return result;
}
std::string nx_osc_props::build_str_list_osc_pans() {
    std::string result = "{";
    for (auto& osc_pan : osc_pans)
        result += std::format("{}, ", std::to_string(osc_pan.load_output()));
    result.pop_back();
    result += "}";
    return result;
}
std::string nx_osc_props::build_str_list_waveforms_i() {
    std::string result = "{";
    for (auto& waveform_i : waveforms_i)
        result += std::format("waveform::{}, ", waveform_to_str(waveform(waveform_i.load_output())));
    result.pop_back();
    result += "}";
    return result;
}

// --------------------------------
// nx_osc

nx_osc::nx_osc(float fs, const nx_osc_props& new_props) : fs{fs} {
    patch(new_props);
}

void nx_osc::note(float midi_note) {
    portamento.set(midi_note, props.portamento_time.audio);
}

void nx_osc::key_on() {
    adsr_pitch.key_on();
    adsr_amp.key_on();
    if (retrigger)
        for (auto& osc : oscs)
            osc.phase(0);
}

void nx_osc::key_off() {
    adsr_pitch.key_off();
    adsr_amp.key_off();
}

void nx_osc::patch(nx_osc_props new_props) {
    props = std::move(new_props);

    for (auto waveform_i : props.waveforms_i) {
        osc_pans.emplace_back();
    }
    for (auto [osc_pan, osc_pan_value] : std::views::zip(osc_pans, props.osc_pans))
        osc_pan.set(osc_pan_value.audio);

    for (auto waveform_i : props.waveforms_i) {
        auto& osc    = oscs.emplace_back(fs);
        osc.waveform = waveform_to_func(waveform(waveform_i.audio));
    }

    retrigger = bool(props.retrigger_i.audio);

    adsr_pitch.set(props.pitch_a.audio, props.pitch_d.audio, props.pitch_s.audio, props.pitch_r.audio);
    adsr_amp.set(props.amp_a.audio, props.amp_d.audio, props.amp_s.audio, props.amp_r.audio);
}

void nx_osc::update_from_props() {
    for (auto [osc_pan, osc_pan_value] : std::views::zip(osc_pans, props.osc_pans)) {
        if (osc_pan_value.has_changed())
            osc_pan.set(osc_pan_value.audio);
    }
    for (auto [osc, waveform_i] : std::views::zip(oscs, props.waveforms_i)) {
        if (waveform_i.update())
            osc.waveform = waveform_to_func(waveform(waveform_i.audio));
    }
    if (props.retrigger_i.update())
        retrigger = bool(props.retrigger_i.audio);
    if (props.pitch_a.has_changed() || props.pitch_d.has_changed() || props.pitch_s.has_changed() ||
        props.pitch_r.has_changed()) {
        adsr_pitch.set(props.pitch_a.audio, props.pitch_d.audio, props.pitch_s.audio, props.pitch_r.audio);
    }
    if (props.amp_a.has_changed() || props.amp_d.has_changed() || props.amp_s.has_changed() ||
        props.amp_r.has_changed()) {
        adsr_amp.set(props.amp_a.audio, props.amp_d.audio, props.amp_s.audio, props.amp_r.audio);
    }
}

std::pair<float, float> nx_osc::tick() {
    props.tick();
    update_from_props();

    float current_midi_note = portamento.tick() + props.tuning.audio;
    float pitch_env         = adsr_pitch.tick();

    std::pair<float, float> output = {0, 0};

    for (auto [osc, tuning, osc_pan] : std::views::zip(oscs, props.osc_tunings, osc_pans)) {
        float osc_freq = clover::midi_to_frequency(current_midi_note + tuning.audio);
        osc_freq       = frequency_by_octave_difference(osc_freq, props.pitch_env_octaves.audio);
        osc_freq       = std::clamp(osc_freq, 0.f, fs * 0.5f);
        osc.freq(osc_freq);
        std::pair<float, float> osc_output = osc_pan.process(osc.tick());
        output.first += osc_output.first;
        output.second += osc_output.second;
    }

    float amp_env = adsr_amp.tick();
    output.first *= amp_env;
    output.second *= amp_env;

    return output;
}
