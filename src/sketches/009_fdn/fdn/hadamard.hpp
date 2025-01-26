#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <cmath>

extern const float hadamard_2x2[4];
extern const float hadamard_4x4[16];
extern const float hadamard_8x8[64];

constexpr float scale(const float x) noexcept {
    // C++23 constexpr std::sqrt
    return 1 / std::sqrt(x);
}

// this can be generalized with a template
// process_hadamard_n<n> for n, where n=2^x for int x
std::array<float, 4> process_hadamard_4(std::array<float, 4>& input);
