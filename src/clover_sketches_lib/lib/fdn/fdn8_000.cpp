// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <numeric>
#include <print>
#include <ranges>

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"
using namespace clover;
using namespace dsp;

#include "fdn8_000.hpp"
#include "hadamard.hpp"

std::string fdn8_props_000::to_str() {
    return std::format(
            "\
{{\n\
    .taps     = {{{}, {}, {}, {}, {}, {}, {}, {}}}, \n\
    .fb_gain  = {}, \n\
    .lpf_cut  = {}, \n\
    .lpf_res  = {}, \n\
    .hpf_cut  = {}, \n\
    .hpf_res  = {}, \n\
}};",
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

fdn8_props_000 fdn8_props_000::taps_mult(float coeff) {
    fdn8_props_000 altered = *this;
    for (auto& tap : altered.taps)
        tap *= coeff;
    return altered;
}
fdn8_props_000 fdn8_props_000::taps_add(float add) {
    fdn8_props_000 altered = *this;
    for (auto& tap : altered.taps)
        tap += add;
    return altered;
}

fdn8_000::fdn8_000(float fs, const fdn8_props_000& props, bool has_gui)
    : fs{fs}, sections{fs, fs, fs, fs, fs, fs, fs, fs}, has_gui{has_gui} {
    patch(props);
}

void fdn8_000::patch(fdn8_props_000 patch_props) {
    props = patch_props;
    update_from_props();
}

void fdn8_000::update_from_props() {
    for (auto [section, tap] : std::views::zip(sections, props.taps)) {
        section.set_lpf(props.lpf_cut, props.lpf_res);
        section.set_hpf(props.hpf_cut, props.hpf_res);
        section.set_time(tap);
        section.set_fb_coeff(props.fb_gain);
    }
}

float fdn8_000::process(float x) {
    std::array<float, 8> delay_outputs;
    delay_outputs.fill(0);

    for (auto [section, delay_output] : std::views::zip(sections, delay_outputs)) {
        float fb = section.prev_feedback * section.fb_coef;
        section.fdl.tick(x + fb);
        delay_output = section.fdl.at(section.fdl_tap);
    }

    std::array<float, 8> feedback = process_hadamard<8>(delay_outputs);

    for (auto [section, fb] : std::views::zip(sections, feedback)) {
        float hpf_result = section.hpf.tick(fb);
        float lpf_result = section.lpf.tick(hpf_result);

        section.prev_feedback = lpf_result;
    }

    float output = std::accumulate(feedback.cbegin(), feedback.cend() - 1, 0.f) * scale(8);

    output *= scale(8);
    return output;
}

float fdn8_000::tick(float x) {
    if (has_gui)
        update_from_props();
    return process(x);
}
