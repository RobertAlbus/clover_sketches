#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <atomic>
#include <cmath>
#include <print>

struct settable {
    std::atomic<float> output;
    std::atomic<float> target;

    float gui;
    float audio;
    std::atomic_bool has_changed_flag;
    float ramp_coef = 0.001;

    settable(float value = 0) : target(value), output(value), gui(value), has_changed_flag(false) {
    }

    settable(const settable& other)
        : target(other.target.load(std::memory_order_acquire)),
          output(other.output.load(std::memory_order_acquire)),
          gui(other.gui),
          has_changed_flag(false) {
    }

    settable& operator=(const settable& other) {
        if (this == &other) {
            std::println("WARNING! copy-assigned settable object to itself.");
        }
        target.store(other.target.load(std::memory_order_release));
        output.store(other.output.load(std::memory_order_release));
        has_changed_flag.store(false, std::memory_order_release);
        gui = other.gui;

        return *this;
    }

    settable& operator=(float value) {
        target.store(value, std::memory_order_release);
        output.store(value, std::memory_order_release);
        gui = value;
        has_changed_flag.store(false, std::memory_order_release);
        return *this;
    }

    ~settable() = default;

    void set(float value) {
        target.store(value, std::memory_order_release);
    }

    bool has_changed() {
        return has_changed_flag.load(std::memory_order_acquire);
    }

    void tick() {
        float out = output.load(std::memory_order_acquire);
        float tar = target.load(std::memory_order_acquire);
        float dif = tar - out;

        audio = out + (ramp_coef * dif);

        has_changed_flag.store(audio != out, std::memory_order_release);
        output.store(audio, std::memory_order_release);
    }
};
