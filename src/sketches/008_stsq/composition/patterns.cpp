// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/patterns.hpp"
#include <unordered_map>
#include <vector>

int X = 2;
int x = 1;
int _ = 0;  // NOLINT

/*
good: can see the pattern
bad:  really sucks to write it.
*/
std::vector<int> pattern::kick_pattern = {X, _, _, _, X, _, _, _, X, _, _, _, X, _, _, _};
std::vector<int> pattern::hhat_pattern = {_, _, X, _, _, _, X, _, _, _, X, _, _, _, X, _};
std::vector<int> pattern::clap_pattern = {_, _, _, _, X, _, _, _, _, _, x, _, X, _, _, _};

/*
 good: easy to write
 bad:  cannot see the pattern
       mitigation: consider displaying the pattern in the UI (non-interactive)
*/
int beat = 4;  // whole:       quarter
int qvr  = 2;  // quaver:      eigth
int sqvr = 2;  // semi-quaver: sixteenth
std::unordered_map<int, int> pattern::kick_map{
        {0 * beat, X},
        {1 * beat, X},
        {2 * beat, X},
        {3 * beat, X},
};

std::unordered_map<int, int> pattern::hhat_map{
        {(0 * beat) + qvr, X},
        {(1 * beat) + qvr, X},
        {(2 * beat) + qvr, X},
        {(3 * beat) + qvr, X},
};

// clang-format off
std::unordered_map<int, int> pattern::clap_map{
        {(1 * beat),       X},
        {(2 * beat) + qvr, x},
        {(3 * beat),       X},
};
// clang-format on

/*

- use unordered_map for now so sinplicity in implementation
- if I need to care about performance, I can always convert to std::vector sorted by key

*/