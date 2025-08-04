#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include <array>
#include <span>

enum struct filter_type {
    lp,
    hp,
    bp,
    notch,
    hs,
    ls,
    eq,
    ap,
};

constexpr std::size_t FILTER_TYPE_SIZE = 8;
using filter_type_list                 = std::array<filter_type, FILTER_TYPE_SIZE>;
using filter_name_list                 = std::array<const char*, FILTER_TYPE_SIZE>;
using filter_func_list =
    std::array<clover::dsp::iir_coeffs (*)(float, float, float, float), FILTER_TYPE_SIZE>;

constexpr filter_type_list filter_types{
    filter_type::lp,
    filter_type::hp,
    filter_type::bp,
    filter_type::notch,
    filter_type::hs,
    filter_type::ls,
    filter_type::eq,
    filter_type::ap,
};

constexpr filter_name_list filter_str{
    "lp",
    "hp",
    "bp",
    "notch",
    "hs",
    "ls",
    "eq",
    "ap",
};

constexpr filter_func_list filter_func = {
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::lpf(fs, f0, reso);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::hpf(fs, f0, reso);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::bpf(fs, f0, reso);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::notch(fs, f0, reso);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::hs(fs, f0, reso, gain);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::ls(fs, f0, reso, gain);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::eq(fs, f0, reso, gain);
    },
    +[](float fs, float f0, float reso, float gain) -> clover::dsp::iir_coeffs {
        return clover::dsp::apf(fs, f0, reso);
    },
};

filter_type draw_filter_type_select(
    const char* id,
    const filter_type& current_filter_type,
    std::span<const enum filter_type> allowed_types = filter_types);
