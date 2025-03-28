// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth.hpp"
#include "filter_block.hpp"
#include "nx_osc.hpp"

nx_osc_props patch_subtractive_synth_oscs{
        .tuning            = 0,
        .portamento_time   = 0,
        .pitch_env_octaves = 0,
        .osc_tunings       = {0, 12.10f, 11.90f},
        .osc_pans          = {0, -0.5f, 0.4f},
        .waveforms_i       = {int(waveform::square), int(waveform::saw), int(waveform::saw)},
        .retrigger_i       = 1,
        .pitch_a           = 0,
        .pitch_d           = 0,
        .pitch_s           = 0,
        .pitch_r           = 0,
        .amp_a             = 100,
        .amp_d             = 3000,
        .amp_s             = 0.1,
        .amp_r             = 2000,
};

filter_block_props patch_subtractive_synth_filter{
        .cutoff               = 100,
        .cutoff_range_octaves = 5,
        .res                  = 0.907,
        .res_range_octaves    = 0,
        .filter_type_i        = int(filter_t::lpf),
        .cut_a                = 100,
        .cut_d                = 3000,
        .cut_s                = 0.3,
        .cut_r                = 3000,
        .res_a                = 1000,
        .res_d                = 1000,
        .res_s                = 0.1,
        .res_r                = 1000,
};

subtractive_synth::subtractive_synth(float fs, const subtractive_synth_props& new_props)
    : fs(fs), osc(fs, new_props.osc_props), filter(fs, new_props.filter_props) {
}

void subtractive_synth::patch(subtractive_synth_props new_props) {
    osc.patch(new_props.osc_props);
    filter.patch(new_props.filter_props);
}

void subtractive_synth::key_on(float midi_note) {
    osc.note(midi_note);
    osc.key_on();
    filter.key_on();
}

void subtractive_synth::key_off() {
    osc.key_off();
    filter.key_off();
}

std::pair<float, float> subtractive_synth::tick() {
    return filter.tick(osc.tick());
}
