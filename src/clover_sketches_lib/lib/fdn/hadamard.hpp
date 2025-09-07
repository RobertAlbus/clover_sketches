#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <ranges>
#include <stdexcept>

// clang-format off
inline constexpr std::array<float, 4> hadamard_2x2 = {
    1,  1,
    1, -1,
};
inline constexpr std::array<float, 16> hadamard_4x4 = {
    1,  1,  1,  1,
    1, -1,  1, -1,
    1,  1, -1, -1,
    1, -1, -1,  1,
};
inline constexpr std::array<float, 64> hadamard_8x8 = {
    1,  1,  1,  1,  1,  1,  1,  1,
    1, -1,  1, -1,  1, -1,  1, -1,
    1,  1, -1, -1,  1, 1,  -1, -1,
    1, -1, -1,  1,  1, -1, -1,  1,
    1,  1,  1,  1, -1, -1, -1, -1,
    1, -1,  1, -1, -1,  1, -1,  1,
    1,  1, -1, -1, -1, -1,  1,  1,
    1, -1, -1,  1, -1,  1,  1, -1,
};
inline constexpr std::array<float, 256> hadamard_16x16 = {
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,
    1,  1, -1, -1,  1,  1, -1, -1,  1,  1, -1, -1,  1, 1,  -1, -1,
    1, -1, -1,  1,  1, -1, -1,  1,  1, -1, -1,  1,  1, -1, -1,  1,
    1,  1,  1,  1, -1, -1, -1, -1,  1,  1,  1,  1, -1, -1, -1, -1,
    1, -1,  1, -1, -1,  1, -1,  1,  1, -1,  1, -1, -1,  1, -1,  1,
    1,  1, -1, -1, -1, -1,  1,  1,  1,  1, -1, -1, -1, -1,  1,  1,
    1, -1, -1,  1, -1,  1,  1, -1,  1, -1, -1,  1, -1,  1,  1, -1,
    1,  1,  1,  1,  1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1,
    1, -1,  1, -1,  1, -1,  1, -1, -1,  1, -1,  1, -1,  1, -1,  1,
    1,  1, -1, -1,  1, 1,  -1, -1, -1, -1,  1,  1, -1, -1,  1,  1,
    1, -1, -1,  1,  1, -1, -1,  1, -1,  1,  1, -1, -1,  1,  1, -1,
    1,  1,  1,  1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1,  1,  1,
    1, -1,  1, -1, -1,  1, -1,  1, -1,  1, -1,  1,  1, -1,  1, -1,
    1,  1, -1, -1, -1, -1,  1,  1, -1, -1,  1,  1,  1,  1, -1, -1,
    1, -1, -1,  1, -1,  1,  1, -1, -1,  1,  1, -1,  1, -1, -1,  1,
};
// clang-format on

inline constexpr float scale(const float x) noexcept {
    return 1 / std::sqrt(x);
}

template <int N>
const std::array<float, N * N>& get_matrix() {
    if constexpr (N == 2) {
        return hadamard_2x2;
    } else if constexpr (N == 4) {
        return hadamard_4x4;
    } else if constexpr (N == 8) {
        return hadamard_8x8;
    } else if constexpr (N == 16) {
        return hadamard_16x16;
    } else {
        throw std::invalid_argument(std::format("get_matrix<N>() for N=%d", N));
    };
}

template <int N>
std::array<float, N> process_hadamard(std::array<float, N>& inputs) {
    std::array<float, N> inverses;
    auto inverted_view = std::views::all(inputs) | std::views::transform([](float x) { return x * -1.f; });
    std::ranges::copy(inverted_view, inverses.begin());

    auto columns = std::views::iota(0, N) | std::views::transform([](size_t i) {
                       return get_matrix<N>() | std::views::drop(i) | std::views::stride(N);
                   });

    std::array<float, N> outputs;
    outputs.fill(0);

    for (auto [output, column] : std::views::zip(outputs, columns)) {
        for (auto [input, inverse, element] : std::views::zip(inputs, inverses, column)) {
            float result = element == 1 ? input : inverse;
            output += result;
        }
    }

    for (auto& value : outputs)
        value *= scale(N);

    return outputs;
}
