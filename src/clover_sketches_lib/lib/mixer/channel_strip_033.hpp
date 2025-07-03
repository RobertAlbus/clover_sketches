#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "lib/audio_frame/audio_frame.hpp"
#include "lib/meter/meter.hpp"

#include "pan_033.hpp"

struct channel_strip_props {
    float gain;
    float pan;
    bool mono = false;
    bool mute = false;
};

struct channel_strip {
    channel_strip_props props;

    pan_033 panning;
    meter_gain_stereo_032 meter;

    channel_strip(float fs, const channel_strip_props& props);
    void patch(channel_strip_props props);
    audio_frame tick(audio_frame x);
    void update_pan();
    std::string to_str();
};
