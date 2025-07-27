// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth_000.hpp"
#include "filter_block_000.hpp"
#include "nx_osc_000.hpp"

subtractive_synth_000::subtractive_synth_000(float fs, const subtractive_synth_props_000& new_props)
    : fs(fs), osc(fs, new_props.osc_props), filter(fs, new_props.filter_props) {
}

void subtractive_synth_000::patch(subtractive_synth_props_000 new_props) {
    osc.patch(new_props.osc_props);
    filter.patch(new_props.filter_props);
}

void subtractive_synth_000::key_on(float midi_note) {
    osc.note(midi_note);
    osc.key_on();
    filter.key_on();
}

void subtractive_synth_000::key_off() {
    osc.key_off();
    filter.key_off();
}

std::pair<float, float> subtractive_synth_000::tick() {
    return filter.tick(osc.tick());
}
