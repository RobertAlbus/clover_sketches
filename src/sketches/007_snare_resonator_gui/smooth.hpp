#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <functional>

#include "clover/dsp/env_linear.hpp"

struct smooth {
    float m_smooth_duration = 10;
    float m_previous        = 0;
    float m_next            = 0;
    bool m_was_updated      = false;

    clover::dsp::env_linear m_env;

    std::function<void(float)> callback = [](float in) {};

    void set(float value) {
        if (value != m_previous) {
            m_was_updated = true;
            m_next        = value;
        }
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
        if (m_was_updated) {
            m_env.set(m_next, m_smooth_duration);
            m_was_updated = false;
        }

        float env_value = m_env.tick();
        if (m_previous != env_value) {
            m_previous = env_value;
            callback(env_value);
        }

        return env_value;
    }
};
