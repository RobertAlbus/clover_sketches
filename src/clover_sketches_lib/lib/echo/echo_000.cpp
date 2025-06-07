// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "clover/dsp/fractional_delay.hpp"

#include "lib/peq/peq.hpp"

#include "echo.hpp"

std::string echo_props_000::to_str() {
    return std::format(
            R"({{
            .delay_samples = {},
            .fb            = {}
        }};)",
            delay_samples,
            fb);
}

echo_000::echo_000(
        float fs,
        float max_length_samples,
        const echo_props_000& new_props,
        const std::array<peq_props_000, peq_000::SIZE>& eq_props)
    : eq{fs, eq_props}, fdl{size_t((2 * max_length_samples) + 1)}, max_length_samples{max_length_samples} {
    patch(new_props);
}
void echo_000::patch(echo_props_000 new_props) {
    props = std::move(new_props);
}

std::pair<float, float> echo_000::tick(float L, float R) {
    return tick({L, R});
}

std::pair<float, float> echo_000::tick(std::pair<float, float> in) {
    float stimulus_L  = in.first + fb.first;
    float stimulus_R  = in.second + fb.second;
    auto [eq_L, eq_R] = eq.tick({stimulus_L, stimulus_R});

    fdl.tick(eq_L, eq_R);

    fb = fdl.at(props.delay_samples);
    fb.first *= props.fb;
    fb.second *= props.fb;

    return fb;
}
