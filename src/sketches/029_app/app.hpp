#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "context.hpp"
#include <stop_token>

struct app {
    context live_ctx;
    context render_ctx{true};

    void start();
    void audio_thread();
    void graphics_thread();
    void audio_render_thread(std::stop_token st);

    std::function<clover::io::callback_status(clover::io::callback_args data)> create_audio_callback(
            bar_grid& grid, signal_graph& comp, sequencers& sqs);
};
