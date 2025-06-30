// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <numbers>

#include "audio_frame.hpp"
#include "pan.hpp"

audio_frame_stereo pan_033::tick(audio_frame_stereo x) const {
    return x * pan_coeffs;
}

void pan_033::set(float value) {
    float angle  = 0.25f * std::numbers::pi_v<float> * (value + 1);
    pan_coeffs.L = std::cos(angle);
    pan_coeffs.R = std::sin(angle);
}

float pan_033::get() const {
    float angle = std::atan2(pan_coeffs.R, pan_coeffs.L);
    return ((4.0f / std::numbers::pi_v<float>)*angle) - 1.0f;
}
