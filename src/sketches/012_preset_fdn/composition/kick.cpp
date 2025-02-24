// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"

#include "kick.hpp"

using namespace clover;
using namespace dsp;

void kick_props::tick() {
    drive.tick();
    trim.tick();
    amp_a.tick();
    amp_d.tick();
    amp_s.tick();
    amp_r.tick();
    cut_a.tick();
    cut_d.tick();
    cut_s.tick();
    cut_r.tick();
    filt_q.tick();
    pitch_a.tick();
    pitch_d.tick();
    pitch_s.tick();
    pitch_r.tick();
    pitch_fundamental.tick();
    pitch_peak.tick();
    cut_fundamental.tick();
    cut_peak.tick();

    if (pitch_fundamental.has_changed() || pitch_peak.has_changed())
        pitch_range = clover::octave_difference_by_frequency(
                pitch_fundamental.output.load(std::memory_order_acquire),
                pitch_peak.output.load(std::memory_order_acquire));

    if (cut_fundamental.has_changed() || cut_peak.has_changed())
        cut_range = clover::octave_difference_by_frequency(
                cut_fundamental.output.load(std::memory_order_acquire),
                cut_peak.output.load(std::memory_order_acquire));
}

std::string kick_props::to_str() {
    return std::format(
            "\
kick_props patch{{     \n\
    .trim              = {}, \n\
    .drive             = {}, \n\
    .amp_a             = {}, \n\
    .amp_d             = {}, \n\
    .amp_s             = {}, \n\
    .amp_r             = {}, \n\
    .cut_a             = {}, \n\
    .cut_d             = {}, \n\
    .cut_s             = {}, \n\
    .cut_r             = {}, \n\
    .filt_q            = {}, \n\
    .pitch_a           = {}, \n\
    .pitch_d           = {}, \n\
    .pitch_s           = {}, \n\
    .pitch_r           = {}, \n\
    .pitch_fundamental = {}, \n\
    .pitch_peak        = {}, \n\
    .cut_fundamental   = {}, \n\
    .cut_peak          = {}, \n\
    .pitch_range       = {}, \n\
    .cut_range         = {}, \n\
}};",
            trim.output.load(std::memory_order_acquire),
            drive.output.load(std::memory_order_acquire),
            amp_a.output.load(std::memory_order_acquire),
            amp_d.output.load(std::memory_order_acquire),
            amp_s.output.load(std::memory_order_acquire),
            amp_r.output.load(std::memory_order_acquire),
            cut_a.output.load(std::memory_order_acquire),
            cut_d.output.load(std::memory_order_acquire),
            cut_s.output.load(std::memory_order_acquire),
            cut_r.output.load(std::memory_order_acquire),
            filt_q.output.load(std::memory_order_acquire),
            pitch_a.output.load(std::memory_order_acquire),
            pitch_d.output.load(std::memory_order_acquire),
            pitch_s.output.load(std::memory_order_acquire),
            pitch_r.output.load(std::memory_order_acquire),
            pitch_fundamental.output.load(std::memory_order_acquire),
            pitch_peak.output.load(std::memory_order_acquire),
            cut_fundamental.output.load(std::memory_order_acquire),
            cut_peak.output.load(std::memory_order_acquire),
            pitch_range,
            cut_range);
}

kick_drum::kick_drum(clover_float fs, kick_props& new_props) : fs(fs), kick_osc(fs) {
    patch(new_props);
}

void kick_drum::patch(kick_props& new_props) {
    props = std::move(new_props);
    kick_osc.freq(props.pitch_fundamental.output.load(std::memory_order_acquire));
    kick_osc.phase(0);

    adsr_cut.set(
            props.cut_a.output.load(std::memory_order_acquire),
            props.cut_d.output.load(std::memory_order_acquire),
            props.cut_s.output.load(std::memory_order_acquire),
            props.cut_r.output.load(std::memory_order_acquire));
    adsr_pitch.set(
            props.pitch_a.output.load(std::memory_order_acquire),
            props.pitch_d.output.load(std::memory_order_acquire),
            props.pitch_s.output.load(std::memory_order_acquire),
            props.pitch_r.output.load(std::memory_order_acquire));
    adsr_amp.set(
            props.amp_a.output.load(std::memory_order_acquire),
            props.amp_d.output.load(std::memory_order_acquire),
            props.amp_s.output.load(std::memory_order_acquire),
            props.amp_r.output.load(std::memory_order_acquire));

    filt.m_coeffs = lpf(fs, props.cut_fundamental.output.load(std::memory_order_acquire), 1);
}

void kick_drum::key_on() {
    key_off();
    kick_osc.phase(0);
    adsr_cut.key_on();
    adsr_pitch.key_on();
    adsr_amp.key_on();
}
void kick_drum::key_off() {
    adsr_cut.key_off();
    adsr_pitch.key_off();
    adsr_amp.key_off();
}

// clang-format off
void kick_drum::update_state() {
    if (props.amp_a.has_changed() ||
        props.amp_d.has_changed() ||
        props.amp_s.has_changed() ||
        props.amp_r.has_changed()) {
        adsr_amp.set(
            props.amp_a.output.load(std::memory_order_acquire),
            props.amp_d.output.load(std::memory_order_acquire),
            props.amp_s.output.load(std::memory_order_acquire),
            props.amp_r.output.load(std::memory_order_acquire));
    }
    if (props.cut_a.has_changed() ||
        props.cut_d.has_changed() ||
        props.cut_s.has_changed() ||
        props.cut_r.has_changed()) {
        adsr_cut.set(
            props.cut_a.output.load(std::memory_order_acquire),
            props.cut_d.output.load(std::memory_order_acquire),
            props.cut_s.output.load(std::memory_order_acquire),
            props.cut_r.output.load(std::memory_order_acquire));
    }
    if (props.pitch_a.has_changed() ||
        props.pitch_d.has_changed() ||
        props.pitch_s.has_changed() ||
        props.pitch_r.has_changed()) {
        adsr_pitch.set(
            props.pitch_a.output.load(std::memory_order_acquire),
            props.pitch_d.output.load(std::memory_order_acquire),
            props.pitch_s.output.load(std::memory_order_acquire),
            props.pitch_r.output.load(std::memory_order_acquire));
    }
}
// clang-format on

clover_float kick_drum::tick() {
    float osc_signal = kick_osc.tick();
    float cutoff_env = adsr_cut.tick();
    float gain_env   = adsr_amp.tick();
    float pitch_env  = adsr_pitch.tick();

    float drive = props.drive.audio;

    float kick_signal = osc_signal * gain_env;
    kick_signal       = std::tanh(kick_signal * drive);
    kick_signal       = filt.tick(kick_signal);

    float kick_pitch =
            frequency_by_octave_difference(props.pitch_fundamental.audio, props.pitch_range * pitch_env);
    kick_osc.freq(kick_pitch);

    float cut = frequency_by_octave_difference(props.cut_fundamental.audio, props.cut_range * cutoff_env);

    filt.m_coeffs = lpf(fs, cut, props.filt_q.audio);

    props.tick();
    update_state();
    return kick_signal * gain_env * props.trim.audio;
};
