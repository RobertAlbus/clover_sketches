// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <atomic>
#include <cmath>
#include <functional>
#include <print>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/math.hpp"
using namespace clover;
using namespace dsp;

#include "composition/settable.hpp"
#include "filter_block.hpp"

filter_t str_to_filter_t(std::string_view str) noexcept {
    auto it = std::find(filter_t_str.begin(), filter_t_str.end(), str);
    return it != filter_t_str.end() ? filter_t::none : static_cast<filter_t>(it - filter_t_str.begin());
}
const char* filter_t_to_str(filter_t filter_type) noexcept {
    return filter_t_str[size_t(filter_type)];
}
std::function<clover::dsp::iir_coeffs(float, float, float)> filter_t_to_func(filter_t filter_type) noexcept {
    return filter_t_func[size_t(filter_type)];
}

std::string filter_block_props::to_str() {
    return std::format(
            "\
filter_block_props patch = {{\n\
    .cutoff               = {}, \n\
    .cutoff_range_octaves = {}, \n\
    .res                  = {}, \n\
    .res_range_octaves    = {}, \n\
    .filter_type_i        = int(filter_t::{}), \n\
    .cut_a                = {}, \n\
    .cut_d                = {}, \n\
    .cut_s                = {}, \n\
    .cut_r                = {}, \n\
    .res_a                = {}, \n\
    .res_d                = {}, \n\
    .res_s                = {}, \n\
    .res_r                = {}, \n\
}};",
            cutoff.load_output(),
            cutoff_range_octaves.load_output(),
            res.load_output(),
            res_range_octaves.load_output(),
            filter_t_to_str(filter_t(filter_type_i.load_output())),
            cut_a.load_output(),
            cut_d.load_output(),
            cut_s.load_output(),
            cut_r.load_output(),
            res_a.load_output(),
            res_d.load_output(),
            res_s.load_output(),
            res_r.load_output());
}

void filter_block_props::tick() {
    cutoff.tick();
    cutoff_range_octaves.tick();
    res.tick();
    res_range_octaves.tick();
    cut_a.tick();
    cut_d.tick();
    cut_s.tick();
    cut_r.tick();
    res_a.tick();
    res_d.tick();
    res_s.tick();
    res_r.tick();
}

filter_block::filter_block(float fs, const filter_block_props& new_props) : fs{fs} {
    patch(new_props);
}

void filter_block::key_on() {
    adsr_cut.key_on();
    adsr_res.key_on();
}

void filter_block::key_off() {
    adsr_cut.key_off();
    adsr_res.key_off();
}

void filter_block::patch(filter_block_props new_props) {
    props = std::move(new_props);

    adsr_cut.set(props.cut_a.audio, props.cut_d.audio, props.cut_s.audio, props.cut_r.audio);
    adsr_res.set(props.res_a.audio, props.res_d.audio, props.res_s.audio, props.res_r.audio);

    auto coeffs = filter_t_func[size_t(props.filter_type_i.audio)](fs, props.cutoff.audio, props.res.audio);
    filt_L.m_coeffs = coeffs;
    filt_R.m_coeffs = coeffs;
}

void filter_block::update_from_props() {
    if (props.cut_a.has_changed() || props.cut_d.has_changed() || props.cut_s.has_changed() ||
        props.cut_r.has_changed()) {
        adsr_cut.set(props.cut_a.audio, props.cut_d.audio, props.cut_s.audio, props.cut_r.audio);
    }
    if (props.res_a.has_changed() || props.res_d.has_changed() || props.res_s.has_changed() ||
        props.res_r.has_changed()) {
        adsr_res.set(props.res_a.audio, props.res_d.audio, props.res_s.audio, props.res_r.audio);
    }
}

std::pair<float, float> filter_block::tick(const std::pair<float, float>& input) {
    return tick(input.first, input.second);
}

std::pair<float, float> filter_block::tick(float in_L, float in_R) {
    props.tick();
    update_from_props();

    float cut_env = adsr_cut.tick();
    float cut =
            frequency_by_octave_difference(props.cutoff.audio, props.cutoff_range_octaves.audio * cut_env);
    float res_env = adsr_res.tick();
    float res     = frequency_by_octave_difference(props.res.audio, props.res_range_octaves.audio * res_env);

    iir_coeffs coeffs = filter_t_func[size_t(props.filter_type_i.audio)](fs, cut, res);
    filt_L.m_coeffs   = coeffs;
    filt_R.m_coeffs   = coeffs;

    return {filt_L.tick(in_L), filt_R.tick(in_R)};
}
