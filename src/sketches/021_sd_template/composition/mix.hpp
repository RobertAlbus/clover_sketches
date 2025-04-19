#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>
#include <unordered_map>
#include <vector>

struct mixer_track {
    const std::string name;
    float gain;
};

std::unordered_map<std::string, std::reference_wrapper<float>> build_audio_mixer(
        std::vector<mixer_track>& mixer_tracks);

std::string to_str(std::vector<mixer_track>& mixer_tracks);