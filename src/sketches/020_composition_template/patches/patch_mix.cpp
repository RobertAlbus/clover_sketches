// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "patch_mix.hpp"

std::string patch_mix_t::to_str() {
    return std::format(
            R"({{
    .kick_send = {},
    .kick_wet  = {},
    .kick_gain = {},

    .bass_gain = {},

    .hh1 = {},
    .hh2 = {},
    .hh3 = {},

    .hh_verb_send = {},
    .hh_verb_wet  = {},
    .hh_bus       = {},

    .hh_drive = {},
    .hh_trim  = {},

    .ride = {},

    .cymbal_bus = {},

    .chord_send = {},
    .chord_wet  = {},
    .chord_dry  = {},
    .chord_sum  = {},

    .pad_send = {},
    .pad_wet  = {},
    .pad_dry  = {},
    .pad_sum  = {},

    .lead_a       = {},
    .lead_b       = {},
    .lead_ringmod = {},
    .lead         = {},
}};)",
            kick_send,
            kick_wet,
            kick_gain,
            bass_gain,
            hh1,
            hh2,
            hh3,
            hh_verb_send,
            hh_verb_wet,
            hh_bus,
            hh_drive,
            hh_trim,
            ride,
            cymbal_bus,
            chord_send,
            chord_wet,
            chord_dry,
            chord_sum,
            pad_send,
            pad_wet,
            pad_dry,
            pad_sum,
            lead_a,
            lead_b,
            lead_ringmod,
            lead

    );
}
