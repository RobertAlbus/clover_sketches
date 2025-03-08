#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <unordered_map>

namespace pattern {

const int X = 2;
const int x = 1;
const int _ = -1;

extern std::unordered_map<int, int> kick_map;
extern std::unordered_map<int, int> hh_map;
extern std::unordered_map<int, std::array<float, 4>> chord_map;

}  // namespace pattern
