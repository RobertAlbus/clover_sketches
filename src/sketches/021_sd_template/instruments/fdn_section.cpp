// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include <print>

#include "fdn_section.hpp"

fdn_section_012::fdn_section_012(float fs) : fs{fs}, fdl{size_t(fs)} {
    lpf.m_coeffs = dsp::lpf(fs, cut_lpf, res_lpf);
    hpf.m_coeffs = dsp::hpf(fs, cut_hpf, res_hpf);
}

void fdn_section_012::set_lpf(float f0, float q) {
    lpf.m_coeffs = dsp::lpf(fs, f0, q);
    cut_lpf      = f0;
    res_lpf      = q;
}
void fdn_section_012::set_hpf(float f0, float q) {
    hpf.m_coeffs = dsp::hpf(fs, f0, q);
    cut_hpf      = f0;
    res_hpf      = q;
}
void fdn_section_012::set_time(float taps) {
    fdl_tap = taps;
}
void fdn_section_012::set_fb_coeff(float fb) {
    fb_coef = fb;
}
