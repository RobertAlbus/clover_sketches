// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "mixer.hpp"

mixer_map_000 build_mixer_map(mixer_list_000& mixer_tracks) {
    mixer_map_000 audio_mixer;

    for (auto& mixer_track : mixer_tracks)
        if (!mixer_track.spacer)
            audio_mixer.emplace(std::pair{mixer_track.name, std::ref(mixer_track.gain)});

    return audio_mixer;
}

std::string to_str(mixer_list_000& mixer_tracks) {
    std::string preset = "{";

    for (auto& mixer_track : mixer_tracks) {
        if (mixer_track.spacer) {
            preset += std::format("\n            {{.spacer = true}},");
        } else {
            preset += std::format(
                    "\n            {{.name = \"{}\", .gain = {}}},", mixer_track.name, mixer_track.gain);
        }
    }

    preset += "\n    };";

    return preset;
}
