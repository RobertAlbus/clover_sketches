#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/io/audio_callback.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"

#include "composition.hpp"
#include "shared_props.hpp"

std::string render_name{"06_gui.wav"};

void AUDIO(shared_props& props) {
    composition comp;
    system_audio_config system;
    stream stream;

    props.composition = &comp;

    props.audio_ready.release();
    props.gui_ready.acquire();

    stream.audio_callback = comp.audio_callback;

    stream.open(stream::settings{
            .device_index_in  = system.no_device(),
            .chan_count_in    = 0,
            .device_index_out = system.default_output().index,
            .chan_count_out   = comp.channel_count_out,
            .sample_rate      = comp.fs_i,
            .latency_ms       = 0});

    stream.start();
    props.gui_intent_to_exit.acquire();
    stream.stop();
    stream.wait_to_complete();
}