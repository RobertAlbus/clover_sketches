// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <numeric>
#include <ranges>

#include "clover/dsp/filter.hpp"
#include "clover/math.hpp"
using namespace clover;

#include "fdn.hpp"
#include "hadamard.hpp"

const float minus_sixty_db = linear_to_db(-60);
const float delay_times[4] = {
        1414.017,  //
        584.121,   //
        841.953,   //
        1019.212,  //
};

fdn_4::fdn_4(float fs)
    : fs(fs),
      sections{
              {24000, delay_times[0]},  //
              {24000, delay_times[1]},  //
              {24000, delay_times[2]},  //
              {24000, delay_times[3]},  //
      } {
    initial_lpf_freq = 2621.206;
    for (auto [section, i] : std::views::zip(sections, std::views::iota(0, 4))) {
        section.hpf.m_coeffs = dsp::hpf(fs, 1000, 0.707);
        section.lpf.m_coeffs = dsp::lpf(fs, initial_lpf_freq, 0.707);

        section.fb_coefficient = 0.97;
    }
}

float fdn_4::tick(float x) {
    std::array<float, 4> delay_outputs{0, 0, 0, 0};

    for (auto [section, delay_output] : std::views::zip(sections, delay_outputs)) {
        float fb = section.prev_feedback * section.fb_coefficient;
        section.fdl.tick(x + fb);
        delay_output = section.fdl.at(section.fdl_tap);
    }

    std::array<float, 4> feedback = process_hadamard_4(delay_outputs);

    for (auto [section, fb] : std::views::zip(sections, feedback)) {
        float hpf_result      = section.hpf.tick(fb);
        float lpf_result      = section.lpf.tick(hpf_result);
        section.prev_feedback = lpf_result;
    }

    float output = std::accumulate(feedback.cbegin(), feedback.cend() - 1, 0.f) * 0.25f;  // scale(4);

    output *= 0.25;
    return output;
}

void fdn_4::lpf_cut(float freq) {
    for (auto& section : sections)
        section.lpf.m_coeffs = lpf(fs, freq, 0.707);
}
void fdn_4::hpf_cut(float freq) {
    for (auto& section : sections)
        section.lpf.m_coeffs = hpf(fs, freq, 0.707);
}