// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <unordered_map>

#include "patterns.hpp"

namespace pattern {

int beat = 4;  // whole:       quarter
int qvr  = 2;  // quaver:      eigth
int sqvr = 1;  // semi-quaver: sixteenth
std::unordered_map<int, int> kick_map{
        {0 * beat, X},
        {1 * beat, X},
        {2 * beat, X},
        {3 * beat, X},

        {(0 * beat) + (3 * sqvr), _},
        {(1 * beat) + (3 * sqvr), _},
        {(2 * beat) + (3 * sqvr), _},
        {(3 * beat) + (3 * sqvr), _},
};

// clang-format on

/*

- use unordered_map for now to prioritize implementation simplicity.
- if I need to care about performance, I can always convert to std::vector sorted by key

*/
}  // namespace pattern
