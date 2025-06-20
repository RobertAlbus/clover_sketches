#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/fdn/fdn8_023.hpp"

// I think I would prefer composition instead of inheritance for props
// - but: the UI relies on a concept that expects e.g. impl.props.taps[i]
struct fdn8_stereo_030_props : public fdn8_props_023 {
    float stereo_spread_taps_octaves = 0;

    // WARNING: heap allocation
    std::string to_str();
};
struct fdn8_stereo_030 {
    fdn8_stereo_030_props props;

    fdn8_023 fdn_L;
    fdn8_023 fdn_R;
    const float max_idx = fdn_L.max_idx;

    std::pair<float, float> tick(std::pair<float, float> x);

    void set_lpf(float f0);
    void set_lpf(float f0, float q);
    void set_hpf(float f0);
    void set_hpf(float f0, float q);

    fdn8_stereo_030(float fs, const fdn8_stereo_030_props& props);

    void patch(fdn8_stereo_030_props props);
};
