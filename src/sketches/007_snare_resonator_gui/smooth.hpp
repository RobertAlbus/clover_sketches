#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <functional>

#include "clover/dsp/env_linear.hpp"

struct smooth {
    float m_smooth_duration = 480;
    float m_previous        = 0;
    clover::dsp::env_linear m_env;

    std::function<void(float)> callback = [](float) {};

    void set(float value) {
        m_env.set(value, m_smooth_duration);
    }

    void init(float value) {
        m_previous = value;
        m_env.set(value, 0);
        callback(value);
    }

    float get() {
        return m_previous;
    }
    float tick() {
        float env_value = m_env.tick();
        if (env_value != m_previous) {
            m_previous = env_value;
            callback(env_value);
        }

        return env_value;
    }
};
