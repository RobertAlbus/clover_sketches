// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"

#include "app.hpp"
#include "context.hpp"

void app::audio_thread() {
    clover::io::system_audio_config system;
    clover::io::stream stream;

    auto audio_callback   = create_audio_callback(live_ctx.grid, live_ctx.graph, live_ctx.sequencers);
    stream.audio_callback = audio_callback;
    // system.print();
    stream.open(clover::io::stream::settings{
            .device_index_in  = system.no_device(),
            .chan_count_in    = 0,
            .device_index_out = system.default_output().index,
            .chan_count_out   = live_ctx.channel_count_out,
            .sample_rate      = int(live_ctx.fs),
            .latency_ms       = 0});

    stream.start();

    live_ctx.gui_intent_to_exit.acquire();
    stream.stop();
    stream.wait_to_complete();
}
