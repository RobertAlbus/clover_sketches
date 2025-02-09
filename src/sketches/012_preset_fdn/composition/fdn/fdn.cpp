// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <numeric>
#include <ranges>

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"
using namespace clover;
using namespace dsp;

#include "composition/settable.hpp"
#include "fdn.hpp"
#include "hadamard.hpp"

void fdn_8_props_012::tick() {
    lpf_cut.tick();
    lpf_res.tick();
    hpf_cut.tick();
    hpf_res.tick();
    for (auto& tap : taps)
        tap.tick();
    for (auto& fb : fb_gains)
        fb.tick();
}
std::string fdn_8_props_012::to_str() {
    return std::format(
            "\
{{\n\
.taps     = {{{}, {}, {}, {}, {}, {}, {}, {}}}, \n\
.fb_gains = {{{}, {}, {}, {}, {}, {}, {}, {}}}, \n\
.lpf_cut  = {}, \n\
.lpf_res  = {}, \n\
.hpf_cut  = {}, \n\
.hpf_res  = {}, \n\
}}",
            taps[0].output,
            taps[1].output,
            taps[2].output,
            taps[3].output,
            taps[4].output,
            taps[5].output,
            taps[6].output,
            taps[7].output,
            fb_gains[0].output,
            fb_gains[1].output,
            fb_gains[2].output,
            fb_gains[3].output,
            fb_gains[4].output,
            fb_gains[5].output,
            fb_gains[6].output,
            fb_gains[7].output,
            lpf_cut.output,
            lpf_res.output,
            hpf_cut.output,
            hpf_res.output);
}

fdn_8_012::fdn_8_012(float fs, const fdn_8_props_012& props)
    : fs{fs}, sections{fs, fs, fs, fs, fs, fs, fs, fs} {
    patch(props);
}

void fdn_8_012::patch(const fdn_8_props_012& props) {
    for (auto [section, fb, tap] : std::views::zip(sections, props.fb_gains, props.taps)) {
        section.set_lpf(props.lpf_cut.output, props.lpf_res.output);
        section.set_hpf(props.hpf_cut.output, props.hpf_res.output);
        section.set_fb_coeff(fb.output);
        section.set_time(tap.output);
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
    props.tick();
    bool hpf_changed = (props.hpf_cut.has_changed() || props.hpf_res.has_changed());
    bool lpf_changed = (props.lpf_cut.has_changed() || props.lpf_res.has_changed());

    for (auto [section, fb, tap] : std::views::zip(sections, props.fb_gains, props.taps)) {
        if (hpf_changed)
            section.set_hpf(props.hpf_cut.output, props.hpf_res.output);
        if (lpf_changed)
            section.set_lpf(props.lpf_cut.output, props.lpf_res.output);
        if (fb.has_changed())
            section.set_fb_coeff(fb.output);
        if (tap.has_changed())
            section.set_time(tap.output);
    }
}
float fdn_8_012::tick(float x) {
    update_state();
    return process(x);
}
