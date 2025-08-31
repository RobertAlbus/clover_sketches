// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth_038.hpp"

#include <algorithm>
#include <format>
#include <ranges>

#include "clover/math.hpp"

std::string subtractive_synth_props_038::to_str() {
    std::string osc_tunings_str = "{";
    std::string osc_pans_str    = "{";
    std::string osc_gains_str   = "{";
    std::string waveforms_str   = "{";
    for (auto i : std::views::iota(0, NUM_OSCS)) {
        osc_tunings_str += std::to_string(osc_tunings[i]);
        osc_pans_str += std::to_string(osc_pans[i]);
        osc_gains_str += std::to_string(osc_gains[i]);
        waveforms_str += std::format("waveform_000::{}", waveform_to_str(waveforms[i]));
        if (i < NUM_OSCS - 1) {
            osc_tunings_str += ", ";
            osc_pans_str += ", ";
            osc_gains_str += ", ";
            waveforms_str += ", ";
        }
    }
    osc_tunings_str += "}";
    osc_pans_str += "}";
    waveforms_str += "}";
    osc_gains_str += "}";

    return std::format(
        R"({{
    .tuning = {},
    .portamento_time = {},
    .pitch_env_octaves = {},
    .retrigger = {},
    .osc_tunings = {},
    .osc_pans = {},
    .osc_gains = {},
    .waveforms = {},
    .pitch_adsr = {},
    .amp_adsr = {},
    .cut_adsr = {},
    .res_adsr = {}
    .cut = {},
    .res = {},
    .cut_env_octaves = {},
    .res_env_octaves = {},
    .filter_type = filter_type::{},
}};)",
        tuning,
        portamento_time,
        pitch_env_octaves,
        retrigger,
        osc_tunings_str,
        osc_pans_str,
        osc_gains_str,
        waveforms_str,
        static_cast<std::string>(pitch_adsr),
        static_cast<std::string>(amp_adsr),
        cut,
        res,
        cut_env_octaves,
        res_env_octaves,
        filter_str[int(filter_type)],
        static_cast<std::string>(cut_adsr),
        static_cast<std::string>(res_adsr));
}

subtractive_synth_038::subtractive_synth_038(clover_float fs, const subtractive_synth_props_038& new_props)
    : fs(fs), oscs{fs, fs, fs, fs, fs, fs}, portamento(fs) {
    patch(new_props);
}

void subtractive_synth_038::note(float midi_note) {
    portamento.set(midi_note, props.portamento_time);
}

void subtractive_synth_038::key_on(float midi_note) {
    note(midi_note);

    adsr_pitch.key_on();
    adsr_amp.key_on();
    adsr_cut.key_on();
    adsr_res.key_on();

    if (props.retrigger) {
        for (auto& osc : oscs) {
            osc.phase(0);
        }
    }
}

void subtractive_synth_038::key_off() {
    adsr_pitch.key_off();
    adsr_amp.key_off();
    adsr_cut.key_off();
    adsr_res.key_off();
}

void subtractive_synth_038::patch(subtractive_synth_props_038 new_props) {
    props = new_props;

    portamento.set(props.portamento_time);

    for (auto i : std::views::iota(0, NUM_OSCS)) {
        oscs[i].waveform = waveform_to_func(props.waveforms[i]);
        osc_pans[i].set(props.osc_pans[i]);
    }

    adsr_pitch.set(props.pitch_adsr.a, props.pitch_adsr.d, props.pitch_adsr.s, props.pitch_adsr.r);
    adsr_amp.set(props.amp_adsr.a, props.amp_adsr.d, props.amp_adsr.s, props.amp_adsr.r);
    adsr_cut.set(props.cut_adsr.a, props.cut_adsr.d, props.cut_adsr.s, props.cut_adsr.r);
    adsr_res.set(props.res_adsr.a, props.res_adsr.d, props.res_adsr.s, props.res_adsr.r);

    // assume filter is always lp, hp, bp, notch, etc. no gain.
    filter.m_coeffs = filter_func[int(props.filter_type)](fs, props.cut, props.res, 0);
}

audio_frame subtractive_synth_038::tick() {
    float current_midi_note = portamento.tick() + props.tuning;
    float pitch_env         = (adsr_pitch.tick() * props.pitch_env_octaves) + input_mod_pitch_octaves;

    audio_frame osc_output = 0;

    for (auto i : std::views::iota(0, NUM_OSCS)) {
        float osc_freq = clover::midi_to_frequency(current_midi_note + props.osc_tunings[i]);
        osc_freq       = clover::frequency_by_octave_difference(osc_freq, pitch_env);
        osc_freq       = std::clamp(osc_freq, 0.f, fs * 0.499f);
        oscs[i].freq(osc_freq);

        audio_frame osc_signal = oscs[i].tick() * props.osc_gains[i];
        osc_output += osc_pans[i].tick(osc_signal);
    }
    osc_output *= (1.f / std::sqrt(float(NUM_OSCS)));

    float cut = adsr_cut.tick() * props.cut_env_octaves;
    cut       = clover::frequency_by_octave_difference(props.cut, cut);
    cut       = std::clamp(cut, 20.f, fs * 0.499f);
    float res = adsr_res.tick() * props.res_env_octaves;
    res       = clover::frequency_by_octave_difference(props.res, res);
    res       = std::max(res, 0.1f);
    // assume filter is always lp, hp, bp, notch, etc. no gain.
    filter.m_coeffs = filter_func[int(props.filter_type)](fs, cut, res, 0);

    audio_frame filtered_output = filter.tick(osc_output.L, osc_output.R);

    return filtered_output * adsr_amp.tick();
}
