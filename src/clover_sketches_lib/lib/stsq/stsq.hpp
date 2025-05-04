#pragma once

#include <functional>
#include <unordered_map>

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

template <typename step_data_t>
struct stsq_000 {
    std::unordered_map<int, step_data_t>* pattern;
    double pattern_duration_samples = 0;
    double pattern_clock            = 0;
    int num_steps                   = 0;
    double step_clock               = 0;
    double step_duration_samples    = 0;
    int step_current                = 0;

    std::function<void(const int clock, const int step, const step_data_t& data)> callback = nullptr;

    void tick() {
        if (callback == nullptr)
            return;

        if (int(step_clock) == 0) {
            auto step_data = pattern->find(step_current);
            if (step_data != pattern->cend()) {
                callback(pattern_clock, step_data->first, step_data->second);
            }
        }

        step_clock += 1;
        if (step_clock >= step_duration_samples) {
            step_clock -= step_duration_samples;
            if (++step_current == num_steps) {
                step_current = 0;
            }
        }

        pattern_clock += 1;
        if (pattern_clock >= pattern_duration_samples) {
            pattern_clock -= pattern_duration_samples;
        }
    }
};
