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

#include "fdn.hpp"
#include "lib/fdn/hadamard.hpp"

std::string fdn8_props_023::to_str() {
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

fdn8_props_023 fdn8_props_023::taps_mult(float coeff) {
    fdn8_props_023 altered = *this;
    for (auto& tap : altered.taps)
        tap *= coeff;
    return altered;
}
fdn8_props_023 fdn8_props_023::taps_add(float add) {
    fdn8_props_023 altered = *this;
    for (auto& tap : altered.taps)
        tap += add;
    return altered;
}

fdn8_023::fdn8_023(float fs, const fdn8_props_023& props) : fs{fs} {
    patch(props);
}

void fdn8_023::patch(fdn8_props_023 patch_props) {
    props = patch_props;
    set_lpf(props.lpf_cut, props.lpf_res);
    set_hpf(props.hpf_cut, props.hpf_res);
}

void fdn8_023::set_lpf(float f0) {
    set_lpf(f0, props.lpf_res);
}
void fdn8_023::set_lpf(float f0, float q) {
    props.lpf_cut = f0;
    props.lpf_res = q;
    for (auto& lpf : lpfs)
        lpf.m_coeffs = dsp::lpf(fs, f0, q);
}
void fdn8_023::set_hpf(float f0) {
    set_hpf(f0, props.hpf_res);
}
void fdn8_023::set_hpf(float f0, float q) {
    props.hpf_cut = f0;
    props.hpf_res = q;
    for (auto& hpf : hpfs)
        hpf.m_coeffs = dsp::hpf(fs, f0, q);
}

float fdn8_023::process(float x) {
    std::array<float, 8> delay_outputs;
    delay_outputs.fill(0);

    for (auto [tap, fb, fdl, delay_output] : std::views::zip(props.taps, fbs, fdls, delay_outputs)) {
        float feedback = fb * props.fb_gain;
        fdl.tick(x + feedback);
        delay_output = fdl.at(tap);
    }

    std::array<float, 8> feedback = process_hadamard<8>(delay_outputs);

    for (auto [fb, lpf, hpf] : std::views::zip(fbs, lpfs, hpfs)) {
        float hpf_result = hpf.tick(fb);
        float lpf_result = lpf.tick(hpf_result);

        fb = lpf_result;
    }

    float output = std::accumulate(feedback.cbegin(), feedback.cend() - 1, 0.f) * scale(8);

    output *= scale(8);
    return output;
}

float fdn8_023::tick(float x) {
    return process(x);
}
