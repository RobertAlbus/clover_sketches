// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth_036.hpp"

#include <algorithm>
#include <format>
#include <ranges>

#include "clover/math.hpp"
#include "lib/peq/peq.hpp"

std::string subtractive_synth_props_036::to_str() {
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
    .pitch_a = {},
    .pitch_d = {},
    .pitch_s = {},
    .pitch_r = {},
    .amp_a = {},
    .amp_d = {},
    .amp_s = {},
    .amp_r = {},
    .cut = {},
    .res = {},
    .cut_env_octaves = {},
    .res_env_octaves = {},
    .filter_type = peq_filter_type::{},
    .cut_a = {},
    .cut_d = {},
    .cut_s = {},
    .cut_r = {},
    .res_a = {},
    .res_d = {},
    .res_s = {},
    .res_r = {}
}};)",
        tuning,
        portamento_time,
        pitch_env_octaves,
        retrigger,
        osc_tunings_str,
        osc_pans_str,
        osc_gains_str,
        waveforms_str,
        pitch_a,
        pitch_d,
        pitch_s,
        pitch_r,
        amp_a,
        amp_d,
        amp_s,
        amp_r,
        cut,
        res,
        cut_env_octaves,
        res_env_octaves,
        peq_filter_str[int(filter_type)],
        cut_a,
        cut_d,
        cut_s,
        cut_r,
        res_a,
        res_d,
        res_s,
        res_r);
}

subtractive_synth_036::subtractive_synth_036(clover_float fs, const subtractive_synth_props_036& new_props)
    : fs(fs), oscs{fs, fs, fs, fs, fs, fs}, portamento(fs) {
    patch(new_props);
}

void subtractive_synth_036::note(float midi_note) {
    portamento.set(midi_note, props.portamento_time);
}

void subtractive_synth_036::key_on(float midi_note) {
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

void subtractive_synth_036::key_off() {
    adsr_pitch.key_off();
    adsr_amp.key_off();
    adsr_cut.key_off();
    adsr_res.key_off();
}

void subtractive_synth_036::patch(subtractive_synth_props_036 new_props) {
    props = new_props;

    portamento.set(props.portamento_time);

    for (auto i : std::views::iota(0, NUM_OSCS)) {
        oscs[i].waveform = waveform_to_func(props.waveforms[i]);
        osc_pans[i].set(props.osc_pans[i]);
    }

    adsr_pitch.set(props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);
    adsr_amp.set(props.amp_a, props.amp_d, props.amp_s, props.amp_r);
    adsr_cut.set(props.cut_a, props.cut_d, props.cut_s, props.cut_r);
    adsr_res.set(props.res_a, props.res_d, props.res_s, props.res_r);

    // assume filter is always lp, hp, bp, notch, etc. no gain.
    filter.m_coeffs = filter_func[int(props.filter_type)](fs, props.cut, props.res, 0);
}

audio_frame subtractive_synth_036::tick() {
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
