// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "channel_strip_033.hpp"
#include "mixer_033.hpp"

mixer_033::mixer_033(float fs, const std::vector<channel_spec>& channel_specs) {
    build(fs, channel_specs);
}

void mixer_033::build(float fs, const list_specs& specs) {
    strips.reserve(specs.size());
    channels.reserve(specs.size());

    for (auto& spec : specs) {
        if (spec.spacer) {
            channels.emplace_back(channel{spec});
        } else {
            channels.emplace_back(channel{spec, channel_strip{fs, spec.props}});
            auto& ch = channels.back();
            strips.emplace(ch.spec.name, ch.strip.value());
        }
    }
}

channel_strip& mixer_033::at(const std::basic_string<char>& key) {
    return strips.at(key);
}

std::string mixer_033::to_str() {
    std::string preset = "{";

    for (auto& channel : channels) {
        if (channel.strip.has_value()) {
            std::string channel_strip_preset = channel.strip.value().to_str();
            channel_strip_preset.pop_back();
            preset += std::format(
                    "\n            {{.name = \"{}\", .props = {}}},",
                    channel.spec.name,
                    channel_strip_preset);
        } else {
            preset += std::format("\n            {{.spacer = true}},");
        }
    }

    preset += "\n    };";

    return preset;
}
