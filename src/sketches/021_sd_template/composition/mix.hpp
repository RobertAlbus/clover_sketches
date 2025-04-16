#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

struct composition_mix {
    std::string to_str();

    float kick_send;
    float kick_wet;
    float kick_gain;

    float chord_send;
    float chord_wet;
    float chord_dry;
    float chord_sum;
};