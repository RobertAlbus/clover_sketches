#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "lib/meter/meter.hpp"

#include "audio_frame.hpp"
#include "graph/instrument/pan.hpp"

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
    audio_frame_stereo tick(audio_frame_stereo x);
    std::string to_str();
};
