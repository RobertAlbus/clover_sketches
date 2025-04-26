// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "mix.hpp"
#include "patches/patches.hpp"

std::unordered_map<std::string, std::reference_wrapper<float>> audio_mixer =
        build_audio_mixer(patch::mix.mixer_tracks);

std::unordered_map<std::string, std::reference_wrapper<float>> build_audio_mixer(
        std::vector<mixer_track>& mixer_tracks) {
    std::unordered_map<std::string, std::reference_wrapper<float>> audio_mixer;
    for (auto& mixer_track : mixer_tracks)
        if (!mixer_track.spacer)
            audio_mixer.emplace(std::pair{mixer_track.name, std::ref(mixer_track.gain)});

    return audio_mixer;
}

std::string to_str(std::vector<mixer_track>& mixer_tracks) {
    std::string preset = "{";

    for (auto& mixer_track : mixer_tracks) {
        if (mixer_track.spacer) {
            preset += std::format("\n            {{.spacer = true}},");
        } else {
            preset += std::format(
                    "\n            {{.name = \"{}\", .gain = {}}},", mixer_track.name, mixer_track.gain);
        }
    }

    preset.pop_back();  // remove trailing comma
    preset += "};";

    return preset;
}
