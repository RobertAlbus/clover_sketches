// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/dsp/fractional_delay.hpp"

#include "lib/peq/peq.hpp"

#include "022_echo.hpp"

std::string echoverb_022_props::to_str() {
    return std::format(
            R"({{
            .delay_samples = {},
            .fb            = {}
        }};)",
            delay_samples,
            fb);
}

echoverb_022::echoverb_022(
        float fs,
        float max_length_samples,
        const echoverb_022_props& new_props,
        const std::array<peq_props_000, peq_000::SIZE>& eq_props)
    : eq{fs, eq_props}, fdl{size_t((2 * max_length_samples) + 1)}, max_length_samples{max_length_samples} {
    patch(new_props);
}
void echoverb_022::patch(echoverb_022_props new_props) {
    props = std::move(new_props);
}

std::pair<float, float> echoverb_022::tick(float L, float R) {
    return tick({L, R});
}

std::pair<float, float> echoverb_022::tick(std::pair<float, float> in) {
    float stimulus_L  = in.first + fb.first;
    float stimulus_R  = in.second + fb.second;
    auto [eq_L, eq_R] = eq.tick({stimulus_L, stimulus_R});

    fdl.tick(eq_L, eq_R);

    fb = fdl.at(props.delay_samples);
    fb.first *= props.fb;
    fb.second *= props.fb;

    return fb;
}
