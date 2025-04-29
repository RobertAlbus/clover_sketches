#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/dsp/fractional_delay.hpp"

#include "instruments/fdn.hpp"
#include "instruments/peq.hpp"

struct echoverb_022_props {
    float delay_samples;
    float verb_dry;
    float verb_wet;
    float fb;

    // WARNING: heap allocation
    // do not call from audio thread.
    std::string to_str();
};

struct echoverb_022 {
    echoverb_022(
            float fs,
            float max_length_samples,
            const echoverb_022_props& new_props,
            const std::array<peq_props, peq::SIZE>& eq_props,
            const fdn_8_props_019& verb_props_L,
            const fdn_8_props_019& verb_props_R);
    void patch(echoverb_022_props new_props);

    echoverb_022_props props;

    peq eq;
    fdn_8_019 verb_L;
    fdn_8_019 verb_R;
    clover::dsp::fdl_lagrange_2 fdl;
    float max_length_samples;

    std::pair<float, float> fb = {0, 0};

    std::pair<float, float> tick(float L, float R);
    std::pair<float, float> tick(std::pair<float, float> in);
};
