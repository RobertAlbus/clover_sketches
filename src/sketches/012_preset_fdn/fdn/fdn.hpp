#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"
#include "clover/math.hpp"
#include <numeric>
using namespace clover;
using namespace dsp;

#include "hadamard.hpp"

float fb_coefficient(float reduction_db, float delay_line_duration_samples, float decay_duration_samples);

struct fdn_section {
    fdn_section(float fs, size_t delay_size_max, float delay_samples)
        : fdl_tap{delay_samples}, fdl{delay_size_max} {
    }

    float prev_feedback{0};
    float fb_coefficient{0};
    float fdl_tap;
    fdl_lagrange fdl;
    filter lpf;
    filter hpf;
};

const float minus_sixty_db = linear_to_db(-60);

template <int N>
struct fdn {
    float fs;
    float initial_lpf_freq = 11621.206;
    std::array<fdn_section, N> sections;
    fdn(float fs, float base_time)
        : fs{fs},
          sections{
                  // this construction only works when there are N fdn_sections defined.
                  // Need a better way to instantiate.
                  fdn_section{fs, 24000, 3 * base_time},   //
                  fdn_section{fs, 24000, 5 * base_time},   //
                  fdn_section{fs, 24000, 7 * base_time},   //
                  fdn_section{fs, 24000, 11 * base_time},  //
                  fdn_section{fs, 24000, 13 * base_time},  //
                  fdn_section{fs, 24000, 17 * base_time},  //
                  fdn_section{fs, 24000, 19 * base_time},  //
                  fdn_section{fs, 24000, 23 * base_time},  //
          } {
        for (auto [section, i] : std::views::zip(sections, std::views::iota(0, N))) {
            section.hpf.m_coeffs = dsp::hpf(fs, 100, 0.707);
            section.lpf.m_coeffs = dsp::lpf(fs, initial_lpf_freq, 0.707);

            section.fb_coefficient = 0.916;
        }
    }

    float tick(float x) {
        std::array<float, N> delay_outputs;
        delay_outputs.fill(0);

        for (auto [section, delay_output] : std::views::zip(sections, delay_outputs)) {
            float fb = section.prev_feedback * section.fb_coefficient;
            section.fdl.tick(x + fb);
            delay_output = section.fdl.at(section.fdl_tap);
        }

        std::array<float, N> feedback = process_hadamard<N>(delay_outputs);

        for (auto [section, fb] : std::views::zip(sections, feedback)) {
            float hpf_result = section.hpf.tick(fb);
            float lpf_result = section.lpf.tick(hpf_result);

            section.prev_feedback = lpf_result;
        }

        float output = std::accumulate(feedback.cbegin(), feedback.cend() - 1, 0.f) * scale(N);

        output *= scale(N);
        return output;
    }

    void lpf_cut(float freq) {
        for (auto& section : sections)
            section.lpf.m_coeffs = lpf(fs, freq, 0.707);
    }

    void hpf_cut(float freq) {
        for (auto& section : sections)
            section.lpf.m_coeffs = hpf(fs, freq, 0.707);
    }
};
