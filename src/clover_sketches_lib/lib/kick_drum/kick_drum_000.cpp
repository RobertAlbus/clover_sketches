// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <format>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/math.hpp"

#include "kick_drum.hpp"

using namespace clover;
using namespace dsp;

std::string kick_props_000::to_str() {
    return std::format(
        R"({{
            .trim              = {},
            .drive             = {},
            .amp_a             = {},
            .amp_d             = {},
            .amp_s             = {},
            .amp_r             = {},
            .cut_a             = {},
            .cut_d             = {},
            .cut_s             = {},
            .cut_r             = {},
            .filt_q            = {},
            .pitch_a           = {},
            .pitch_d           = {},
            .pitch_s           = {},
            .pitch_r           = {},
            .pitch_fundamental = {},
            .cut_fundamental   = {},
            .pitch_range       = {},
            .cut_range         = {},
        }};)",
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
        cut_fundamental,
        pitch_range,
        cut_range);
}

kick_drum_000::kick_drum_000(clover_float fs, const kick_props_000& new_props) : fs(fs), kick_osc(fs) {
    patch(new_props);
}

void kick_drum_000::patch(kick_props_000 new_props) {
    props = std::move(new_props);
    kick_osc.freq(props.pitch_fundamental);
    kick_osc.phase(0);
    adsr_cut.set(props.cut_a, props.cut_d, props.cut_s, props.cut_r);
    adsr_pitch.set(props.pitch_a, props.pitch_d, props.pitch_s, props.pitch_r);
    adsr_amp.set(props.amp_a, props.amp_d, props.amp_s, props.amp_r);

    filt.m_coeffs = lpf(fs, props.cut_fundamental, 1);
}

void kick_drum_000::key_on() {
    key_off();
    kick_osc.phase(0);
    adsr_cut.key_on();
    adsr_pitch.key_on();
    adsr_amp.key_on();
}
void kick_drum_000::key_off() {
    adsr_cut.key_off();
    adsr_pitch.key_off();
    adsr_amp.key_off();
}

clover_float kick_drum_000::tick() {
    float osc_signal = kick_osc.tick();
    float cutoff_env = adsr_cut.tick();
    float gain_env   = adsr_amp.tick();
    float pitch_env  = adsr_pitch.tick();

    float kick_signal = osc_signal * gain_env;
    kick_signal       = std::tanh(kick_signal * props.drive);
    kick_signal       = filt.tick(kick_signal);

    float kick_pitch = frequency_by_octave_difference(props.pitch_fundamental, props.pitch_range * pitch_env);
    kick_osc.freq(kick_pitch);

    float cut = std::clamp(
        frequency_by_octave_difference(props.cut_fundamental, props.cut_range * cutoff_env), 10.f, 20000.f);

    filt.m_coeffs = lpf(fs, cut, props.filt_q);

    return kick_signal * gain_env * props.trim;
};
