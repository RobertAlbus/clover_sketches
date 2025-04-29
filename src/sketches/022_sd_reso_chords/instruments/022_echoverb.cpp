// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "clover/dsp/fractional_delay.hpp"

#include "fdn.hpp"
#include "peq.hpp"

#include "022_echoverb.hpp"

std::string echoverb_022_props::to_str() {
    return std::format(
            R"({{
            .delay_samples = {},
            .verb_dry      = {},
            .verb_wet      = {},
            .fb            = {}
        }};)",
            delay_samples,
            verb_dry,
            verb_wet,
            fb);
}

echoverb_022::echoverb_022(
        float fs,
        float max_length_samples,
        const echoverb_022_props& new_props,
        const std::array<peq_props, peq::SIZE>& eq_props,
        const fdn_8_props_019& verb_props_L,
        const fdn_8_props_019& verb_props_R)
    : eq{fs, eq_props},
      verb_L{fs, verb_props_L},
      verb_R{fs, verb_props_R},
      fdl{size_t((2 * max_length_samples) + 1)},
      max_length_samples{max_length_samples} {
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
    float verb_wet_L  = verb_L.tick(eq_L * props.verb_wet);
    float verb_wet_R  = verb_R.tick(eq_R * props.verb_wet);

    float verb_dry_L = eq_L * props.verb_dry;
    float verb_dry_R = eq_R * props.verb_dry;

    float verb_sum_L = verb_dry_L + verb_wet_L;
    float verb_sum_R = verb_dry_R + verb_wet_R;

    fdl.tick(verb_sum_L, verb_sum_R);

    fb = fdl.at(props.delay_samples);
    fb.first *= props.fb;
    fb.second *= props.fb;

    return fb;
}
