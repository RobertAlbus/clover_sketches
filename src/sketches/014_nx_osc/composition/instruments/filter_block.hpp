#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <functional>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
using namespace clover;
using namespace dsp;

#include "composition/settable.hpp"

enum struct filter_t {
    lpf,
    bpf,
    hpf,
    none,
};

constexpr std::array<const char*, 4> filter_t_str{
        "lpf",
        "bpf",
        "hpf",
        "none",
};

const std::array<std::function<clover::dsp::iir_coeffs(float, float, float)>, 4> filter_t_func{
        dsp::lpf, dsp::bpf, dsp::hpf, [](float, float, float) { return clover::dsp::iir_coeffs{}; }};

filter_t str_to_filter_t(std::string_view str) noexcept;
const char* filter_t_to_str(filter_t filter_type) noexcept;
std::function<clover::dsp::iir_coeffs(float, float, float)> filter_t_to_func(filter_t filter_type) noexcept;

struct filter_block_props {
    settable cutoff;
    settable cutoff_range_octaves;
    settable res;
    settable res_range_octaves;
    settable_int filter_type_i;

    settable cut_a;
    settable cut_d;
    settable cut_s;
    settable cut_r;

    settable res_a;
    settable res_d;
    settable res_s;
    settable res_r;

    std::string to_str();
    void tick();
};

struct filter_block {
    filter_block_props props;
    filter_block(float fs, const filter_block_props& new_props);

    float fs;
    env_adsr adsr_cut;
    env_adsr adsr_res;
    filter filt_L;
    filter filt_R;

    void key_on();
    void key_off();
    void patch(filter_block_props new_props);
    void update_from_props();
    std::pair<float, float> tick(float in_L, float in_R);
    std::pair<float, float> tick(const std::pair<float, float>& input);
};
