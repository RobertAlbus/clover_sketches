
#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"

using namespace clover;
using namespace dsp;

struct resonator_section {
    float m_fs       = 0;
    float m_freq     = 0;
    float m_index    = 0;
    float m_gain_out = 0;
    float m_gain_fb  = 0;

    float m_lpf_cut = 0;
    float m_lpf_q   = 0.707;
    float m_hpf_cut = 0;
    float m_hpf_q   = 0.707;

    fdl_lagrange m_resonator;
    filter m_lpf_fb_filt;
    filter m_hpf_fb_filt;

    resonator_section(float fs, size_t max_size) : m_fs{fs}, m_resonator{max_size} {
    }

    float tick(float in) {
        float resonator_tap = m_resonator.at(m_index);
        float output        = resonator_tap * m_gain_out;
        float feedback      = resonator_tap;
        feedback            = m_lpf_fb_filt.tick(feedback);
        feedback            = m_hpf_fb_filt.tick(feedback);

        m_resonator.tick(in + (feedback * m_gain_fb));

        return output;
    }

    void freq(float fhz) {
        m_freq  = fhz;
        m_index = m_fs / fhz;
    }

    void lpf_set(float cut, float q) {
        m_lpf_cut              = cut;
        m_lpf_q                = q;
        m_lpf_fb_filt.m_coeffs = lpf(m_fs, cut, q);
    }
    void lpf_cut(float cut) {
        m_lpf_cut              = cut;
        m_lpf_fb_filt.m_coeffs = lpf(m_fs, cut, m_lpf_q);
    }

    void hpf_set(float cut, float q) {
        m_hpf_cut              = cut;
        m_hpf_q                = q;
        m_hpf_fb_filt.m_coeffs = hpf(m_fs, cut, q);
    }
    void hpf_cut(float cut) {
        m_hpf_cut              = cut;
        m_hpf_fb_filt.m_coeffs = hpf(m_fs, cut, m_hpf_q);
    }
};
