#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "channel_strip.hpp"

struct mixer_033 {
    struct channel_spec {
        const std::string name;
        channel_strip_props props;
        bool spacer = false;
    };
    struct channel {
        channel_spec spec;
        std::optional<channel_strip> strip;
    };

    using map_strip     = std::unordered_map<std::string, std::reference_wrapper<channel_strip>>;
    using list_channels = std::vector<channel>;
    map_strip strips;
    list_channels channels;

    using list_specs = std::vector<channel_spec>;
    mixer_033(float fs, const list_specs& channel_specs);
    void build(float fs, const list_specs& specs);

    channel_strip& at(const std::basic_string<char>& key);

    std::string to_str();
};
