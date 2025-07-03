#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
using namespace clover;
using namespace dsp;

enum struct filter_t_000 {
    lpf,
    bpf,
    hpf,
    none,
};

constexpr std::array<const char*, 4> filter_t_str_000{
        "lpf",
        "bpf",
        "hpf",
        "none",
};

const std::array<std::function<clover::dsp::iir_coeffs(float, float, float)>, 4> filter_t_func_000{
        dsp::lpf, dsp::bpf, dsp::hpf, [](float, float, float) { return clover::dsp::iir_coeffs{}; }};

filter_t_000 str_to_filter_t(std::string_view str) noexcept;
const char* filter_t_to_str(filter_t_000 filter_type) noexcept;
std::function<clover::dsp::iir_coeffs(float, float, float)> filter_t_to_func(
        filter_t_000 filter_type) noexcept;

struct filter_block_props_000 {
    float cutoff;
    float cutoff_range_octaves;
    float res;
    float res_range_octaves;
    filter_t_000 filter_type;

    float cut_a;
    float cut_d;
    float cut_s;
    float cut_r;

    float res_a;
    float res_d;
    float res_s;
    float res_r;

    std::string to_str();
};

struct filter_block_000 {
    filter_block_props_000 props;
    filter_block_000(float fs, const filter_block_props_000& new_props);

    float fs;
    env_adsr adsr_cut;
    env_adsr adsr_res;
    filter filt_L;
    filter filt_R;

    void key_on();
    void key_off();
    void patch(filter_block_props_000 new_props);
    std::pair<float, float> tick(float in_L, float in_R);
    std::pair<float, float> tick(const std::pair<float, float>& input);
};
