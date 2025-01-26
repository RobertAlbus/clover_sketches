// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <array>
#include <cmath>
#include <ranges>

#include "hadamard.hpp"

// https://en.wikipedia.org/wiki/Hadamard_matrix#Sylvester's_construction
// clang-format off

const float hadamard_2x2[4] = {
    1,  1,
    1, -1,
};

const float hadamard_4x4[16] = {
    1,  1,  1,  1,
    1, -1,  1, -1,
    1,  1, -1, -1,
    1, -1, -1,  1,
};

const float hadamard_8x8[64] = {
    1,  1,  1,  1,  1,  1,  1,  1,
    1, -1,  1, -1,  1, -1,  1, -1,
    1,  1, -1, -1,  1, 1,  -1, -1,
    1, -1, -1,  1,  1, -1, -1,  1,
    1,  1,  1,  1, -1, -1, -1, -1,
    1, -1,  1, -1, -1,  1, -1,  1,
    1,  1, -1, -1, -1, -1,  1,  1,
    1, -1, -1,  1, -1,  1,  1, -1,
};
// clang-format on

// this can be generalized with a template
// process_hadamard_n<n> for n, where n=2^x for int x
int count = 0;
std::array<float, 4> process_hadamard_4(std::array<float, 4>& inputs) {
    std::array<float, 4> inverses;
    auto inverted_view = std::views::all(inputs) | std::views::transform([](float x) { return x * -1.f; });
    std::ranges::copy(inverted_view, inverses.begin());

    auto columns = std::views::iota(0, 4) | std::views::transform([](size_t i) {
                       return hadamard_4x4 | std::views::drop(i) | std::views::stride(4);
                   });

    std::array<float, 4> outputs = {0, 0, 0, 0};
    for (auto [output, column] : std::views::zip(outputs, columns)) {
        for (auto [input, inverse, element] : std::views::zip(inputs, inverses, column)) {
            float result = element == 1 ? input : inverse;
            output += result;
        }
    }

    for (auto& value : outputs)
        value *= scale(4);

    return outputs;
}
