#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/oscillator.hpp"
#include "clover/io/audio_callback.hpp"

using namespace clover;
using namespace dsp;
using namespace io;

struct composition {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 60 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator osc{fs};
    float gain = 0;

    composition() {
        osc.freq(200);
    };

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        L = osc.tick() * gain;
        ;
        R = L;

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
