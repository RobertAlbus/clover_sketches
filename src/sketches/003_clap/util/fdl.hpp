#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <format>
// #include <iostream>
#include <vector>

#include "clover/dsp/interpolate_lagrange.hpp"
#include "clover/float.hpp"

struct s03_circular_buffer_2 {
    std::vector<clover_float> m_underlying;
    size_t m_current;
    size_t m_length;

    s03_circular_buffer_2(size_t size);

    void tick(clover_float L, clover_float R);
    size_t length();
    std::pair<clover_float&, clover_float&> operator[](size_t idx);
};

s03_circular_buffer_2::s03_circular_buffer_2(size_t length)
    : m_underlying(2 * length, 0), m_length(length), m_current((length * 2) - 2) {
}

void s03_circular_buffer_2::tick(clover_float L, clover_float R) {
    m_current += 2;
    if (m_current == 2 * m_length) {
        m_current = 0;
    }
    m_underlying[m_current]     = L;
    m_underlying[m_current + 1] = R;
    // std::cout << "s03_circular_buffer_2::tick   " << L << " " << R << std::endl;
    // std::cout << "m_underlying[m_current & +1]  " << m_underlying[m_current] << " "
    //           << m_underlying[m_current + 1] << std::endl;
}

size_t s03_circular_buffer_2::length() {
    return m_length;
}

std::pair<clover_float&, clover_float&> s03_circular_buffer_2::operator[](size_t idx) {
    if (idx < 0 || idx >= m_length)
        throw std::out_of_range(std::format("out of range: circular_buffer_2::operator[{}]", idx));

    size_t interleaved_idx = 2 * idx;

    if (m_current < interleaved_idx)
        return {
                m_underlying[m_current + (2 * m_length) - interleaved_idx],     //
                m_underlying[m_current + (2 * m_length) - interleaved_idx + 1]  //
        };
    return {
            m_underlying[m_current - interleaved_idx],     //
            m_underlying[m_current - interleaved_idx + 1]  //
    };
}

struct s03_fdl_lagrange_2 {
    clover_float m_max_idx;
    s03_circular_buffer_2 m_buffer;

    s03_fdl_lagrange_2(size_t length);
    void tick(clover_float L, clover_float R);
    size_t length();
    std::pair<clover_float, clover_float> at(clover_float idx);
};

s03_fdl_lagrange_2::s03_fdl_lagrange_2(size_t length)
    : m_buffer{length}, m_max_idx(static_cast<clover_float>(length) - 3) {
}

void s03_fdl_lagrange_2::tick(clover_float L, clover_float R) {
    // std::cout << "s03_fdl_lagrange_2::tick      " << L << " " << R << std::endl;
    m_buffer.tick(L, R);
}

size_t s03_fdl_lagrange_2::length() {
    return m_buffer.length();
}

std::pair<clover_float, clover_float> s03_fdl_lagrange_2::at(clover_float idx) {
    if (idx < 2)
        return m_buffer[0];

    if (idx > m_max_idx)
        idx = m_max_idx;

    auto p2   = static_cast<size_t>(std::floor(idx));
    size_t p1 = p2 - 1;
    size_t p3 = p1 + 1;
    size_t p4 = p2 + 2;

    clover_float interpolation = idx - std::floor(idx);

    auto [p1_L, p1_R] = m_buffer[p1];
    auto [p2_L, p2_R] = m_buffer[p2];
    auto [p3_L, p3_R] = m_buffer[p3];
    auto [p4_L, p4_R] = m_buffer[p4];

    return {
            clover::dsp::interpolate_lagrange(p1_L, p2_L, p3_L, p4_L, interpolation),
            clover::dsp::interpolate_lagrange(p1_R, p2_R, p3_R, p4_R, interpolation),
    };
}
