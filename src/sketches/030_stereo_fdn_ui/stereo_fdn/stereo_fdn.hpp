#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/fdn/fdn8_023.hpp"
#include <concepts>
#include <format>
#include <ranges>

template <typename T>
concept has_stereo_spread = requires(T t, float f) {
    { t.stereo_spread(f) } -> std::same_as<void>;
};

struct fdn8_030_stereo_props : public fdn8_props_023 {
    float stereo_spread_taps_octaves = 0;

    // WARNING: heap allocation
    std::string to_str() {
        return std::format(
                R"({{
        .stereo_spread_taps_octaves = {},
        .taps     = {{{}, {}, {}, {}, {}, {}, {}, {}}},
        .fb_gain  = {},
        .lpf_cut  = {},
        .lpf_res  = {},
        .hpf_cut  = {},
        .hpf_res  = {},
    }};)",
                stereo_spread_taps_octaves,
                taps[0],
                taps[1],
                taps[2],
                taps[3],
                taps[4],
                taps[5],
                taps[6],
                taps[7],
                fb_gain,
                lpf_cut,
                lpf_res,
                hpf_cut,
                hpf_res);
    }
};
struct fdn8_030_stereo {
    fdn8_030_stereo_props props;

    fdn8_023 fdn_L;
    fdn8_023 fdn_R;
    const float max_idx = fdn_L.max_idx;

    std::pair<float, float> tick(std::pair<float, float> x) {
        // should [1] == 0?
        fdn_L.props.fb_gain                      = props.fb_gain;
        fdn_R.props.fb_gain                      = props.fb_gain;
        constexpr std::array<float, 8> direction = {0, 0, 1, -1, 1, -1, 1, -1};
        for (auto [tap_L, tap_R, tap_base, direction] :
             std::views::zip(fdn_L.props.taps, fdn_R.props.taps, props.taps, direction)) {
            tap_L = tap_base * std::exp2(direction * props.stereo_spread_taps_octaves);
            tap_R = tap_base * std::exp2(-direction * props.stereo_spread_taps_octaves);
        }
        return {fdn_L.tick(x.first), fdn_R.tick(x.second)};
    }

    void set_lpf(float f0) {
        set_lpf(f0, props.lpf_res);
    }
    void set_lpf(float f0, float q) {
        fdn_L.set_lpf(f0, q);
        fdn_R.set_lpf(f0, q);
        props.lpf_cut = f0;
        props.lpf_res = q;
    }
    void set_hpf(float f0) {
        set_hpf(f0, props.hpf_res);
    }
    void set_hpf(float f0, float q) {
        fdn_L.set_hpf(f0, q);
        fdn_R.set_hpf(f0, q);
        props.hpf_cut = f0;
        props.hpf_res = q;
    }

    fdn8_030_stereo(float fs, const fdn8_030_stereo_props& props)
        : fdn_L{fs, static_cast<fdn8_props_023>(props)},
          fdn_R{fs, static_cast<fdn8_props_023>(props)},
          props{std::move(props)} {
    }

    void patch(fdn8_030_stereo_props props) {
        fdn_L.patch(static_cast<fdn8_props_023>(props));
        fdn_R.patch(static_cast<fdn8_props_023>(props));
        props = std::move(props);
    }
};