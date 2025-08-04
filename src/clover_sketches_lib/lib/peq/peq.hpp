#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstddef>
#include <string>
#include <utility>

#include "clover/dsp/filter.hpp"
#include "lib/_atom/filter_type.hpp"

struct peq_props_000 {
    float freq       = 20000;
    float reso       = 0.707;
    float gain       = 0;
    bool enabled     = false;
    filter_type type = filter_type::lp;

    // allocates on the heap.
    // do not call from audio thread.
    std::string to_str();
};

bool operator==(const peq_props_000& a, const peq_props_000& b);
bool operator!=(const peq_props_000& a, const peq_props_000& b);
peq_props_000 lerp(const peq_props_000& a, const peq_props_000& b, float lerp_amount);

struct peq_000 {
    static const size_t SIZE = 4;
    std::array<peq_props_000, peq_000::SIZE> props;
    std::array<clover::dsp::filter_2, peq_000::SIZE> filters;
    float fs;

    peq_000(float fs, const std::array<peq_props_000, peq_000::SIZE>& new_props);
    void patch(std::array<peq_props_000, peq_000::SIZE> new_props);
    std::string to_str();

    std::pair<float, float> tick(float in);
    std::pair<float, float> tick(float in_L, float in_R);
    std::pair<float, float> tick(std::pair<float, float> in);

    void set(size_t i, float new_freq, float new_reso, float new_gain);
    void set(size_t i, const peq_props_000& props);
    void freq(size_t i, float value);
    void reso(size_t i, float value);
    void gain(size_t i, float value);
    void type(size_t i, filter_type value);
    void enabled(size_t i, bool value = true);
    void calculate_coefficients(size_t i);
};

struct peq_gui_model {
    peq_gui_model(std::array<peq_props_000, peq_000::SIZE> new_props) : props{new_props} {
        dirty.fill(false);
    }
    alignas(64) std::array<peq_props_000, peq_000::SIZE> props;
    alignas(64) std::array<bool, peq_000::SIZE> dirty;
};

void update_peq_from_gui(peq_gui_model& gui_model, peq_000& audio_model);
