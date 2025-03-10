// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <atomic>
#include <numeric>
#include <print>
#include <ranges>

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"
using namespace clover;
using namespace dsp;

#include "composition/settable.hpp"
#include "fdn.hpp"
#include "hadamard.hpp"

fdn_8_props_012 fdn_patch = {
        .taps    = {655.98645, 1307.852, 199.59464, 1894.7349, 199.59464, 3198.709, 4893.918, 9262.27},
        .fb_gain = 0.8310298,
        .lpf_cut = 599.3303,
        .lpf_res = 0.707,
        .hpf_cut = 105.487625,
        .hpf_res = 0.707,
};

void fdn_8_props_012::tick() {
    lpf_cut.tick();
    lpf_res.tick();
    hpf_cut.tick();
    hpf_res.tick();
    fb_gain.tick();
    for (auto& tap : taps) {
        tap.tick();
    }
}
std::string fdn_8_props_012::to_str() {
    return std::format(
            "\
fdn_8_props_012 patch = {{\n\
    .taps     = {{{}, {}, {}, {}, {}, {}, {}, {}}}, \n\
    .fb_gain  = {}, \n\
    .lpf_cut  = {}, \n\
    .lpf_res  = {}, \n\
    .hpf_cut  = {}, \n\
    .hpf_res  = {}, \n\
}};",
            taps[0].output.load(std::memory_order_acquire),
            taps[1].output.load(std::memory_order_acquire),
            taps[2].output.load(std::memory_order_acquire),
            taps[3].output.load(std::memory_order_acquire),
            taps[4].output.load(std::memory_order_acquire),
            taps[5].output.load(std::memory_order_acquire),
            taps[6].output.load(std::memory_order_acquire),
            taps[7].output.load(std::memory_order_acquire),
            fb_gain.output.load(std::memory_order_acquire),
            lpf_cut.output.load(std::memory_order_acquire),
            lpf_res.output.load(std::memory_order_acquire),
            hpf_cut.output.load(std::memory_order_acquire),
            hpf_res.output.load(std::memory_order_acquire));
}

fdn_8_012::fdn_8_012(float fs, const fdn_8_props_012& props)
    : fs{fs}, sections{fs, fs, fs, fs, fs, fs, fs, fs} {
    patch(props);
}

void fdn_8_012::patch(const fdn_8_props_012& patch_props) {
    props = patch_props;
    for (auto [section, tap] : std::views::zip(sections, props.taps)) {
        section.set_lpf(props.lpf_cut.audio, props.lpf_res.audio);
        section.set_hpf(props.hpf_cut.audio, props.hpf_res.audio);
        section.set_time(tap.audio);
        section.set_fb_coeff(props.fb_gain.audio);
    }
}

float fdn_8_012::process(float x) {
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
void fdn_8_012::update_state() {
    if (props.hpf_cut.has_changed() || props.hpf_res.has_changed()) {
        for (auto& section : sections)
            section.set_hpf(props.hpf_cut.audio, props.hpf_res.audio);
    }
    if (props.lpf_cut.has_changed() || props.lpf_res.has_changed()) {
        for (auto& section : sections)
            section.set_lpf(props.lpf_cut.audio, props.lpf_res.audio);
    }
    if (props.fb_gain.has_changed()) {
        for (auto& section : sections)
            section.set_fb_coeff(props.fb_gain.audio);
    }

    for (auto [i, section, tap] : std::views::zip(std::views::iota(0, 8), sections, props.taps)) {
        if (tap.has_changed()) {
            section.set_time(tap.audio);
        }
    }
}
float fdn_8_012::tick(float x) {
    props.tick();
    update_state();
    return process(x);
}
