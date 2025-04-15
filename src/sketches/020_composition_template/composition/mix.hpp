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

    float bass_gain;

    float hh1;
    float hh2;
    float hh3;

    float hh_verb_send;
    float hh_verb_wet;
    float hh_bus;

    // drive only on hh
    float hh_drive;
    float hh_trim;

    float ride;

    float cymbal_bus;

    float chord_send;
    float chord_wet;
    float chord_dry;
    float chord_sum;

    float pad_send;
    float pad_wet;
    float pad_dry;
    float pad_sum;

    float lead_a;
    float lead_b;
    float lead_ringmod;
    float lead;
};