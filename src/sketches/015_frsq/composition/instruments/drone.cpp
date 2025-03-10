// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "drone.hpp"
#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/nx_osc.hpp"

nx_osc_props patch_drone_oscs{
        .tuning            = 0,
        .portamento_time   = 0,
        .pitch_env_octaves = 0,
        .osc_tunings       = {0, 12.10f, 11.90f},
        .osc_pans          = {0, -0.3f, 0.3f},
        .waveforms_i       = {int(waveform::sine), int(waveform::saw), int(waveform::saw)},
        .pitch_a           = 1,
        .pitch_d           = 1,
        .pitch_s           = 0,
        .pitch_r           = 1,
        .amp_a             = 1000,
        .amp_d             = 1000,
        .amp_s             = 0.1,
        .amp_r             = 1000,
};

filter_block_props patch_drone_filter{
        .cutoff               = 1500,
        .cutoff_range_octaves = 0,
        .res                  = 0.507,
        .res_range_octaves    = 0,
        .filter_type_i        = int(filter_t::lpf),
        .cut_a                = 5000,
        .cut_d                = 8000,
        .cut_s                = 0.5,
        .cut_r                = 1000,
        .res_a                = 1000,
        .res_d                = 1000,
        .res_s                = 0.1,
        .res_r                = 1000,
};

drone_synth::drone_synth(float fs) : fs(fs), osc(fs, patch_drone_oscs), filter(fs, patch_drone_filter) {
}

void drone_synth::key_on(float midi_note) {
    osc.note(midi_note);
    osc.key_on();
    filter.key_on();
}

void drone_synth::key_off() {
    osc.key_off();
    filter.key_off();
}

std::pair<float, float> drone_synth::tick() {
    return filter.tick(osc.tick());
}
