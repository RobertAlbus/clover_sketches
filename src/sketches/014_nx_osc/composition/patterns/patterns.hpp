#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <unordered_map>
#include <vector>

namespace pattern {

const int X = 2;
const int x = 1;
const int _ = 0;

extern std::vector<int> kick_pattern;
extern std::unordered_map<int, int> kick_map;

}  // namespace pattern
