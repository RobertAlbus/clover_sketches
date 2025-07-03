// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth_034.hpp"
#include "filter_block_000.hpp"

subtractive_synth_034::subtractive_synth_034(float fs, const subtractive_synth_props_034& new_props)
    : fs(fs), osc(fs, new_props.osc_props), filter(fs, new_props.filter_props) {
}

void subtractive_synth_034::patch(subtractive_synth_props_034 new_props) {
    osc.patch(new_props.osc_props);
    filter.patch(new_props.filter_props);
}

void subtractive_synth_034::key_on(float midi_note) {
    osc.note(midi_note);
    osc.key_on();
    filter.key_on();
}

void subtractive_synth_034::key_off() {
    osc.key_off();
    filter.key_off();
}

audio_frame subtractive_synth_034::tick() {
    return filter.tick(osc.tick());
}
