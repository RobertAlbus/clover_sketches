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
    gain.tick();
    amp_a.tick();
    amp_d.tick();
    amp_s.tick();
    amp_r.tick();
    cut_a.tick();
    cut_d.tick();
    cut_s.tick();
    cut_r.tick();
    pitch_a.tick();
    pitch_d.tick();
    pitch_s.tick();
    pitch_r.tick();
    pitch_fundamental.tick();
    pitch_peak.tick();
    cut_fundamental.tick();
    cut_peak.tick();

    if (pitch_fundamental.has_changed() || pitch_peak.has_changed())
        pitch_range = clover::octave_difference_by_frequency(pitch_fundamental.output, pitch_peak.output);

    if (cut_fundamental.has_changed() || cut_peak.has_changed())
        cut_range = clover::octave_difference_by_frequency(cut_fundamental.output, cut_peak.output);
}

std::string kick_props::to_str() {
    return std::format(
            "\
kick_props patch{{     \n\
    .gain              = {}, \n\
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
            gain.output,
            drive.output,
            amp_a.output,
            amp_d.output,
            amp_s.output,
            amp_r.output,
            cut_a.output,
            cut_d.output,
            cut_s.output,
            cut_r.output,
            filt_q.output,
            pitch_a.output,
            pitch_d.output,
            pitch_s.output,
            pitch_r.output,
            pitch_fundamental.output,
            pitch_peak.output,
            cut_fundamental.output,
            cut_peak.output,
            pitch_range,
            cut_range);
}

kick_props basic_patch{
        .gain              = 0.5,
        .drive             = 1.4,
        .amp_a             = 30,
        .amp_d             = 5000,
        .amp_s             = 0,
        .amp_r             = 1000,
        .cut_a             = 1,
        .cut_d             = 2000,
        .cut_s             = 0,
        .cut_r             = 2000,
        .filt_q            = 1,
        .pitch_a           = 1,
        .pitch_d           = 3000,
        .pitch_s           = 0,
        .pitch_r           = 1000,
        .pitch_fundamental = 50,
        .pitch_peak        = 200,
        .cut_fundamental   = 100,
        .cut_peak          = 200,
        .pitch_range       = 2,  // dont WANT to set this manually because it's a computed prop;
        .cut_range         = 1,  // dont WANT to set this manually because it's a computed prop;
};

kick_props example_patch{
        .gain              = 0.5,
        .drive             = 1.4,
        .amp_a             = 30,
        .amp_d             = 5953.263,
        .amp_s             = 0.16666669,
        .amp_r             = 1000,
        .cut_a             = 1,
        .cut_d             = 4167.5825,
        .cut_s             = 0,
        .cut_r             = 2000,
        .filt_q            = 1,
        .pitch_a           = 1,
        .pitch_d           = 2764,
        .pitch_s           = 0,
        .pitch_r           = 77,
        .pitch_fundamental = 50,
        .pitch_peak        = 200,
        .cut_fundamental   = 100,
        .cut_peak          = 200,
        .pitch_range       = 2,
        .cut_range         = 1,
};

kick_drum::kick_drum(clover_float fs) : fs(fs), kick_osc(fs) {
    patch(basic_patch);
}

void kick_drum::patch(kick_props& new_props) {
    props = new_props;
    kick_osc.freq(props.pitch_fundamental.output);
    kick_osc.phase(0);

    adsr_cut.set(props.cut_a.output, props.cut_d.output, props.cut_s.output, props.cut_r.output);
    adsr_pitch.set(props.pitch_a.output, props.pitch_d.output, props.pitch_s.output, props.pitch_r.output);
    adsr_amp.set(props.amp_a.output, props.amp_d.output, props.amp_s.output, props.amp_r.output);

    filt.m_coeffs = lpf(fs, props.cut_fundamental.output, 1);
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
            props.amp_a.output,
            props.amp_d.output,
            props.amp_s.output,
            props.amp_r.output);
    }
    if (props.cut_a.has_changed() ||
        props.cut_d.has_changed() ||
        props.cut_s.has_changed() ||
        props.cut_r.has_changed()) {
        adsr_cut.set(
            props.cut_a.output,
            props.cut_d.output,
            props.cut_s.output,
            props.cut_r.output);
    }
    if (props.pitch_a.has_changed() ||
        props.pitch_d.has_changed() ||
        props.pitch_s.has_changed() ||
        props.pitch_r.has_changed()) {
        adsr_pitch.set(
            props.pitch_a.output,
            props.pitch_d.output,
            props.pitch_s.output,
            props.pitch_r.output);
    }
}
// clang-format on

clover_float kick_drum::tick() {
    float osc_signal = kick_osc.tick();
    float cutoff_env = adsr_cut.tick();
    float gain_env   = adsr_amp.tick();
    float pitch_env  = adsr_pitch.tick();

    float kick_signal = osc_signal * gain_env;
    kick_signal       = std::tanh(kick_signal * props.drive.output);
    kick_signal       = filt.tick(kick_signal);

    float kick_pitch =
            frequency_by_octave_difference(props.pitch_fundamental.output, props.pitch_range * pitch_env);
    kick_osc.freq(kick_pitch);

    float cut = frequency_by_octave_difference(props.cut_fundamental.output, props.cut_range * cutoff_env);

    filt.m_coeffs = lpf(fs, cut, props.filt_q.output);

    props.tick();
    update_state();
    return kick_signal * gain_env * props.gain.output;
};
