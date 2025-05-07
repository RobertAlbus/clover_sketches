// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <functional>
#include <print>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/math.hpp"
using namespace clover;
using namespace dsp;

#include "filter_block.hpp"

filter_t_000 str_to_filter_t(std::string_view str) noexcept {
    auto it = std::find(filter_t_str_000.begin(), filter_t_str_000.end(), str);
    return it != filter_t_str_000.end() ? filter_t_000::none
                                        : static_cast<filter_t_000>(it - filter_t_str_000.begin());
}
const char* filter_t_to_str(filter_t_000 filter_type) noexcept {
    return filter_t_str_000[size_t(filter_type)];
}
std::function<clover::dsp::iir_coeffs(float, float, float)> filter_t_to_func(
        filter_t_000 filter_type) noexcept {
    return filter_t_func_000[size_t(filter_type)];
}

std::string filter_block_props_000::to_str() {
    return std::format(
            "\
filter_block_props patch = {{\n\
    .cutoff               = {}, \n\
    .cutoff_range_octaves = {}, \n\
    .res                  = {}, \n\
    .res_range_octaves    = {}, \n\
    .filter_type          = filter_t::{}, \n\
    .cut_a                = {}, \n\
    .cut_d                = {}, \n\
    .cut_s                = {}, \n\
    .cut_r                = {}, \n\
    .res_a                = {}, \n\
    .res_d                = {}, \n\
    .res_s                = {}, \n\
    .res_r                = {}, \n\
}};",
            cutoff,
            cutoff_range_octaves,
            res,
            res_range_octaves,
            filter_t_to_str(filter_type),
            cut_a,
            cut_d,
            cut_s,
            cut_r,
            res_a,
            res_d,
            res_s,
            res_r);
}

filter_block_000::filter_block_000(float fs, const filter_block_props_000& new_props) : fs{fs} {
    patch(new_props);
}

void filter_block_000::key_on() {
    adsr_cut.key_on();
    adsr_res.key_on();
}

void filter_block_000::key_off() {
    adsr_cut.key_off();
    adsr_res.key_off();
}

void filter_block_000::patch(filter_block_props_000 new_props) {
    props = std::move(new_props);

    adsr_cut.set(props.cut_a, props.cut_d, props.cut_s, props.cut_r);
    adsr_res.set(props.res_a, props.res_d, props.res_s, props.res_r);

    auto coeffs     = filter_t_func_000[size_t(props.filter_type)](fs, props.cutoff, props.res);
    filt_L.m_coeffs = coeffs;
    filt_R.m_coeffs = coeffs;
}

std::pair<float, float> filter_block_000::tick(const std::pair<float, float>& input) {
    return tick(input.first, input.second);
}

std::pair<float, float> filter_block_000::tick(float in_L, float in_R) {
    float cut_env = adsr_cut.tick();
    float cut     = frequency_by_octave_difference(props.cutoff, props.cutoff_range_octaves * cut_env);
    float res_env = adsr_res.tick();
    float res     = frequency_by_octave_difference(props.res, props.res_range_octaves * res_env);

    iir_coeffs coeffs = filter_t_func_000[size_t(props.filter_type)](fs, cut, res);
    filt_L.m_coeffs   = coeffs;
    filt_R.m_coeffs   = coeffs;

    return {filt_L.tick(in_L), filt_R.tick(in_R)};
}
