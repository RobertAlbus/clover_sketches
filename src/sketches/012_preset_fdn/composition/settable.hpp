#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/env_linear.hpp"

struct settable {
    float output_previous = 0;
    float output          = 0;
    float input;
    bool was_set = false;

    float smooth_duration = 100;

    clover::dsp::env_linear m_env;

    settable(settable& other) {
        output          = other.output;
        output_previous = other.output_previous;
        m_env.set(other.output, 0);
    }
    settable(float value = 0) {
        output          = value;
        output_previous = value;
        m_env.set(value, 0);
    }

    void set(float value) {
        input   = value;
        was_set = true;
    }

    bool has_changed() {
        return output != output_previous;
    }

    void tick() {
        if (was_set) {
            was_set = false;
            m_env.set(input, 100);
        }
        output_previous = output;
        output          = m_env.tick();
    }
};
