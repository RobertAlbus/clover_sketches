// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "mix.hpp"

std::string composition_mix::to_str() {
    return std::format(
            R"({{
    kick_send = {},
    kick_wet  = {},
    kick_gain = {},

    chord_send = {},
    chord_wet  = {},
    chord_dry  = {},
    chord_sum  = {},
}};)",
            kick_send,
            kick_wet,
            kick_gain,
            chord_send,
            chord_wet,
            chord_dry,
            chord_sum);
}
