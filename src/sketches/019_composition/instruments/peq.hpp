#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <cstddef>
#include <string>
#include <utility>

#include "clover/dsp/filter.hpp"

enum struct peq_filter_type {
    lp,
    hp,
    bp,
    notch,
    hs,
    ls,
    eq,
};
constexpr std::array<peq_filter_type, 7> peq_filter_types{
        peq_filter_type::lp,
        peq_filter_type::hp,
        peq_filter_type::bp,
        peq_filter_type::notch,
        peq_filter_type::hs,
        peq_filter_type::ls,
        peq_filter_type::eq,
};
constexpr std::array<const char*, 7> peq_filter_str{
        "lp",
        "hp",
        "bp",
        "notch",
        "hs",
        "ls",
        "eq",
};

constexpr std::array<clover::dsp::iir_coeffs (*)(float, float, float, float), 7> filter_func = {
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
};

struct peq_props {
    float freq           = 20000;
    float reso           = 0.707;
    float gain           = 0;
    bool enabled         = false;
    peq_filter_type type = peq_filter_type::lp;

    // allocates on the heap.
    // do not call from audio thread.
    std::string to_str();
};

bool operator==(const peq_props& a, const peq_props& b);
bool operator!=(const peq_props& a, const peq_props& b);
peq_props lerp(const peq_props& a, const peq_props& b, float lerp_amount);

struct peq {
    static const size_t SIZE = 4;
    std::array<peq_props, peq::SIZE> props;
    std::array<clover::dsp::filter_2, peq::SIZE> filters;
    float fs;

    peq(float fs, const std::array<peq_props, peq::SIZE>& new_props);
    void patch(std::array<peq_props, peq::SIZE> new_props);
    std::string to_str();

    std::pair<float, float> tick(float in);
    std::pair<float, float> tick(float in_L, float in_R);
    std::pair<float, float> tick(std::pair<float, float> in);

    void set(size_t i, float new_freq, float new_reso, float new_gain);
    void freq(size_t i, float value);
    void reso(size_t i, float value);
    void gain(size_t i, float value);
    void type(size_t i, peq_filter_type value);
    void enabled(size_t i, bool value = true);
    void calculate_coefficients(size_t i);
};

struct peq_gui_model {
    peq_gui_model(std::array<peq_props, peq::SIZE> new_props) : props{new_props} {
        dirty.fill(false);
    }
    alignas(64) std::array<peq_props, peq::SIZE> props;
    alignas(64) std::array<bool, peq::SIZE> dirty;
};

void update_peq_from_gui(peq_gui_model& gui_model, peq& audio_model);
