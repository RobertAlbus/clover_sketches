// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <unordered_map>
#include <vector>

#include "patterns.hpp"

namespace pattern {

/*
good: can see the pattern
bad:  really sucks to write it.
*/
std::vector<int> kick_pattern = {X, _, _, _, X, _, _, _, X, _, _, _, X, _, _, _};
std::vector<int> hhat_pattern = {_, _, X, _, _, _, X, _, _, _, X, _, _, _, X, _};
std::vector<int> clap_pattern = {_, _, _, _, X, _, _, _, _, _, x, _, X, _, _, _};

/*
 good: easy to write
 bad:  cannot see the pattern
       mitigation: consider displaying the pattern in the UI (non-interactive)
*/
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

std::unordered_map<int, int> hhat_map{
        {(0 * beat) + qvr, X},
        {(1 * beat) + qvr, X},
        {(2 * beat) + qvr, X},
        {(3 * beat) + qvr, X},
        //
        {0 * beat, _},
        {1 * beat, _},
        {2 * beat, _},
        {3 * beat, _},

};

// clang-format off
std::unordered_map<int, int> clap_map{
        {(1 * beat),       X},
        {(3 * beat),       X},
        {(5 * beat),       X},
        {(6 * beat) + qvr, x},
        {(7 * beat),       X},
};
// clang-format on

/*

- use unordered_map for now to prioritize implementation simplicity.
- if I need to care about performance, I can always convert to std::vector sorted by key

*/
}  // namespace pattern
