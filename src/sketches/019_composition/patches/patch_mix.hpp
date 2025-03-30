#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

struct patch_mix_t {
    float kick_send = 0.8f;
    float kick_wet = 1.f;
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

    float lead_a       = 0.5f;
    float lead_b       = 0.5f;
    float lead_ringmod = 0.5f;
    float lead         = 0.3f;
};
