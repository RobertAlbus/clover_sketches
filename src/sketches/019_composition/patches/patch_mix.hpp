#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

struct patch_mix_t {
    float kick_send = 0.8f;
    float kick_wet  = 1.f;
    float kick_gain = 1.f;

    float bass_gain = 0.45;

    float hh1 = 1.f;
    float hh2 = 0.6f;
    float hh3 = 0.4f;

    float hh_verb_send = 1.f;
    float hh_verb_wet  = 0.5f;
    float hh_bus       = 1.f;

    // drive only on hh
    float hh_drive = 1.8f;
    float hh_trim  = 0.808f;

    float ride = 0.4f;

    float cymbal_bus = 0.7f;

    float chord_send = 0.8f;
    float chord_wet  = 1.f;
    float chord_sum  = 0.13f;

    float pad_send = 0.8f;
    float pad_wet  = 1.f;
    float pad_sum  = 0.13f;

    float lead_a       = 0.5f;
    float lead_b       = 0.5f;
    float lead_ringmod = 0.5f;
    float lead         = 0.3f;

    std::string to_str();
};

inline patch_mix_t mix_patch{
        .kick_send = 0.6904762,
        .kick_wet  = 0.5952381,
        .kick_gain = 0.92857146,

        .bass_gain = 0.7380953,

        .hh1 = 1.0476191,
        .hh2 = 0.26190472,
        .hh3 = 0.23809528,

        .hh_verb_send = 0.9523809,
        .hh_verb_wet  = 0.6428572,
        .hh_bus       = 1.0714285,

        .hh_drive = 1.5238096,
        .hh_trim  = 0.7619047,

        .ride = 0.11490679,

        .cymbal_bus = 0.71428573,

        .chord_send = 1,
        .chord_wet  = 0.47619045,
        .chord_sum  = 0.16666663,

        .pad_send = 1.0108695,
        .pad_wet  = 1,
        .pad_sum  = 0.53260875,

        .lead_a       = 0.28571427,
        .lead_b       = 0,
        .lead_ringmod = 0,
        .lead         = 0.57142854,
};