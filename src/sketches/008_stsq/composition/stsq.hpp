#pragma once

#include <functional>
#include <unordered_map>

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

template <typename step_data_t>
struct stsq {
    std::unordered_map<int, step_data_t>* pattern;
    double pattern_duration_samples = 0;
    double pattern_clock            = 0;
    int num_divisions               = 0;

    std::function<void(const step_data_t&)> callback = nullptr;

    void tick() {
        if (callback == nullptr)
            return;

        auto step_data = pattern->find(int(pattern_clock));
        if (step_data != pattern->cend()) {
            auto& value = step_data->second;
            callback(value);
        }

        pattern_clock += 1;
        if (pattern_clock >= pattern_duration_samples)
            pattern_clock -= pattern_duration_samples;
    }
};
