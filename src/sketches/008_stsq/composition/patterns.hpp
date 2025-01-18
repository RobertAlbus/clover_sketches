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
extern std::vector<int> hhat_pattern;
extern std::vector<int> clap_pattern;

extern std::unordered_map<int, int> kick_map;
extern std::unordered_map<int, int> hhat_map;
extern std::unordered_map<int, int> clap_map;

}  // namespace pattern
