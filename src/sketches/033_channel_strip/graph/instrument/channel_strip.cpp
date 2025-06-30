// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>
#include <string>

#include "lib/meter/meter.hpp"

#include "audio_frame.hpp"
#include "channel_strip.hpp"

channel_strip::channel_strip(float fs, const channel_strip_props& props) : meter{fs} {
    patch(props);
}

void channel_strip::patch(channel_strip_props new_props) {
    props = std::move(new_props);
    panning.set(props.pan);
}

audio_frame_stereo channel_strip::tick(audio_frame_stereo x) {
    x = panning.tick(x);
    x *= props.gain;

    if (props.mono) {
        x = x.mono();
    }

    meter.tick(x.L, x.R);

    return props.mute ? x * 0 : x;
}

std::string channel_strip::to_str() {
    return std::format(
            "{{ .gain = {}, .pan = {}, .mono = {}, .mute = {}}};",
            props.gain,
            props.pan,
            props.mono,
            props.mute);
}
