#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "filter_block.hpp"
#include "nx_osc.hpp"

struct subtractive_synth_props {
    nx_osc_props osc_props;
    filter_block_props filter_props;
};

struct subtractive_synth {
    float fs;
    nx_osc osc;
    filter_block filter;

    subtractive_synth(float fs, const subtractive_synth_props& new_props);
    void patch(subtractive_synth_props new_props);
    void key_on(float midi_note);
    void key_off();
    std::pair<float, float> tick();
};
