#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/dsp/fractional_delay.hpp"

#include "lib/peq/peq.hpp"

struct echo_props_000 {
    float delay_samples;
    float fb;

    // WARNING: heap allocation
    // do not call from audio thread.
    std::string to_str();
};

struct echo_000 {
    echo_000(
            float fs,
            float max_length_samples,
            const echo_props_000& new_props,
            const std::array<peq_props_000, peq_000::SIZE>& eq_props);
    void patch(echo_props_000 new_props);

    echo_props_000 props;

    peq_000 eq;

    clover::dsp::fdl_lagrange_2 fdl;
    float max_length_samples;

    std::pair<float, float> fb = {0, 0};

    std::pair<float, float> tick(float L, float R);
    std::pair<float, float> tick(std::pair<float, float> in);
};
