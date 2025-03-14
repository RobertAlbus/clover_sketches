#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/nx_osc.hpp"
struct subtractive_synth {
    float fs;
    nx_osc osc;
    filter_block filter;

    subtractive_synth(float fs);
    void key_on(float midi_note);
    void key_off();
    std::pair<float, float> tick();
};
