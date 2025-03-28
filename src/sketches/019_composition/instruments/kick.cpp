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
            trim,
            drive,
            amp_a,
            amp_d,
            amp_s,
            amp_r,
            cut_a,
            cut_d,
            cut_s,
            cut_r,
            filt_q,
            pitch_a,
            pitch_d,
            pitch_s,
            pitch_r,
            pitch_fundamental,
            pitch_peak,
            cut_fundamental,
            cut_peak,
            pitch_range,
            cut_range);
}

kick_drum::kick_drum(clover_float fs, const kick_props& new_props) : fs(fs), kick_osc(fs) {
    patch(new_props);
}

void kick_drum::patch(kick_props new_props) {
    props = std::move(new_props);
    kick_osc.freq(props.pitch_fundamental);
    kick_osc.phase(0);

    adsr_cut.set(props.cut_a, props.cut_d, props.cut_s, props.cut_r);
    adsr_pitch.set(props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);
    adsr_amp.set(props.amp_a, props.amp_d, props.amp_s, props.amp_r);

    filt.m_coeffs = lpf(fs, props.cut_fundamental, 1);
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

clover_float kick_drum::tick() {
    float osc_signal = kick_osc.tick();
    float cutoff_env = adsr_cut.tick();
    float gain_env   = adsr_amp.tick();
    float pitch_env  = adsr_pitch.tick();

    float drive = props.drive;

    float kick_signal = osc_signal * gain_env;
    kick_signal       = std::tanh(kick_signal * drive);
    kick_signal       = filt.tick(kick_signal);

    float kick_pitch = frequency_by_octave_difference(props.pitch_fundamental, props.pitch_range * pitch_env);
    kick_osc.freq(kick_pitch);

    float cut = frequency_by_octave_difference(props.cut_fundamental, props.cut_range * cutoff_env);

    filt.m_coeffs = lpf(fs, cut, props.filt_q);

    return kick_signal * gain_env * props.trim;
};
