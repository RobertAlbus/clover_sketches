#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "filter_block_000.hpp"
#include "nx_osc_034.hpp"

struct subtractive_synth_props_034 {
    nx_osc_props_034 osc_props;
    filter_block_props_000 filter_props;
};

struct subtractive_synth_034 {
    float fs;
    nx_osc_034 osc;
    filter_block_000 filter;

    subtractive_synth_034(float fs, const subtractive_synth_props_034& new_props);
    void patch(subtractive_synth_props_034 new_props);
    void key_on(float midi_note);
    void key_off();
    audio_frame tick();
};
