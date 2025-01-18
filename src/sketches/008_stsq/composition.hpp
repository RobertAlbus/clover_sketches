#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/io/audio_callback.hpp"

#import "composition/clap.hpp"
#import "composition/cymbal.hpp"
#import "composition/kick.hpp"

using namespace clover;
using namespace io;

struct composition {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    kick_drum kick;
    cymbal hh;
    hand_clap clap;

    composition() = default;

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        std::tie(L, R) = tick();

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };

    std::pair<clover_float, clover_float> tick();
};
