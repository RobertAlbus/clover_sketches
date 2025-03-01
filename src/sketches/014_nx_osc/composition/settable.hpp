#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <atomic>
#include <cmath>
#include <print>

// template for any discrete type? e.g. int, const char*, etc.
struct settable_int {
    std::atomic<int> output;
    int gui;
    int audio;

    settable_int(int value = 0)
        : output(value), gui(value), audio(value)  {}

    settable_int(const settable_int& other)
        : output(other.output.load(std::memory_order_acquire)),
          gui(other.gui),
          audio(other.audio)
           {}

    settable_int& operator=(const settable_int& other) {
        if (this == &other) {
            std::println("WARNING! copy-assigned settable_int object to itself.");
        }
        output.store(other.output.load(std::memory_order_release));
        gui = other.gui;
        audio = other.audio;
        return *this;
    }

    settable_int& operator=(int value) {
        output.store(value, std::memory_order_release);
        gui = value;
        audio = value;
        return *this;
    }

    ~settable_int() = default;

    void set(int value) {
        output.store(value, std::memory_order_release);
    }
    /*
        called update() because tick and side effect are coupled.
        - don't want to call `tick()` in `instrument_props.tick()` method.
        - should call `settable_int.update()` in `instrument.update_from_props()`
        - this is prettier than storing previous audio state, diffing, and updating
        
        NOTE: to be called from audio thread
    */
    bool update() {
        int out = output.load(std::memory_order_acquire);
        bool changed = audio == out;
        audio = out;

        return changed;
    }
};


struct settable {
    std::atomic<float> output;
    std::atomic<float> target;

    float gui;
    float audio;
    std::atomic_bool has_changed_flag;
    float ramp_coef = 0.001;

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

        // use epsilon for this check
        has_changed_flag.store(audio != out, std::memory_order_release);
        output.store(audio, std::memory_order_release);
    }

    settable(float value = 0)
        : target(value), output(value), gui(value), audio(value), has_changed_flag(false) {
    }

    settable(const settable& other)
        : target(other.target.load(std::memory_order_acquire)),
          output(other.output.load(std::memory_order_acquire)),
          gui(other.gui),
          audio(other.audio),
          has_changed_flag(false) {
    }

    settable& operator=(const settable& other) {
        if (this == &other) {
            std::println("WARNING! copy-assigned settable object to itself.");
        }
        target.store(other.target.load(std::memory_order_release));
        output.store(other.output.load(std::memory_order_release));
        has_changed_flag.store(false, std::memory_order_release);
        gui   = other.gui;
        audio = other.audio;

        return *this;
    }

    settable& operator=(float value) {
        target.store(value, std::memory_order_release);
        output.store(value, std::memory_order_release);
        gui   = value;
        audio = value;
        has_changed_flag.store(false, std::memory_order_release);
        return *this;
    }

    ~settable() = default;
};
