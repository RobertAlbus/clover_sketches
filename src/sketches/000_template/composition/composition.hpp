#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstdint>

struct composition {
    std::pair<float, float> tick();

    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int channel_count_out = 2;
    double bpm            = 132;
    double spm            = fs * 60;
    double spb            = spm / bpm;
    double bar            = spb * 4;
    int_fast64_t duration = int_fast64_t(bar * 16 * 1000) + 1;
};
