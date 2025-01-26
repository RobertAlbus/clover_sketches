#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/io/audio_callback.hpp"
#include "clover/math.hpp"

#include "composition/clap.hpp"
#include "composition/cymbal.hpp"
#include "composition/kick.hpp"
#include "composition/sequencers.hpp"
#include "fdn/fdn.hpp"

using namespace clover;
using namespace io;

struct composition {
    std::pair<clover_float, clover_float> tick();

    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int_fast64_t duration = 2000 * fs_i;
    int channel_count_out = 2;

    kick_drum kick{fs};
    cymbal hhat{fs};
    hand_clap clap{fs};
    fdn_4 reverb_L{fs};
    fdn_4 reverb_R{fs};
    float loop_mix     = 1;
    float reverb_mix   = 0.8;
    float verb_in_gain = 1;

    sequencers stsqs{fs, 138, kick, clap, hhat};

    composition() = default;

    float mix_kick = db_to_linear(-8);
    float mix_clap = db_to_linear(-6);
    float mix_hhat = db_to_linear(0);

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        std::tie(L, R) = tick();

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
