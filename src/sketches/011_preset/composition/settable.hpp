#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/env_linear.hpp"

struct settable {
    float input_previous = 0;
    float input          = 0;

    float output_previous = 0;
    float output          = 0;

    float smooth_duration = 10;

    clover::dsp::env_linear m_env;

    settable(settable& other) {
        input           = other.input;
        input_previous  = other.input_previous;
        output          = other.output;
        output_previous = other.output_previous;
        m_env.set(other.input, 0);
    }
    settable(float value = 0) {
        input           = value;
        input_previous  = value;
        output          = value;
        output_previous = value;
        m_env.set(value, 0);
    }

    bool has_changed() {
        return output != output_previous;
    }

    void tick() {
        if (input != input_previous) {
            m_env.set(input, smooth_duration);
            input_previous = input;
        }
        output_previous = output;
        output          = m_env.tick();
    }
};
