#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>
#include <unordered_map>
#include <vector>

struct mixer_track_000 {
    const std::string name = "";
    float gain             = 0.5;
    bool spacer            = false;
};

using mixer_map_000  = std::unordered_map<std::string, std::reference_wrapper<float>>;
using mixer_list_000 = std::vector<mixer_track_000>;

mixer_map_000 build_mixer_map(mixer_list_000& mixer_tracks);
std::string to_str(mixer_list_000& mixer_tracks);
