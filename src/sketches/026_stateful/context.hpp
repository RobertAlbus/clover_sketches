#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <semaphore>

#include "clover/io/stream.hpp"

#include "graph/graph.hpp"
#include "gui/view.hpp"
#include "lib/logging/logger.hpp"
#include "sequence/sequencers.hpp"

const float base_duration_bars = 32;
struct context {
    context(bool render_mode = false)
        : duration_bars(render_mode ? base_duration_bars * 2.f : base_duration_bars) {
        should_loop = !render_mode;
    }
    float fs  = 48000;
    float bpm = 124;
    float duration_bars;
    bool should_loop = true;

    int channel_count_out = 2;
    const std::string render_name{"026_stateful"};

    log_bus_000 logger;

    bar_grid grid{fs, bpm, duration_bars, should_loop};
    signal_graph graph{grid};
    sequencers sequencers{graph, grid, logger};
    view view{graph, logger};

    clover::io::stream* stream = nullptr;

    std::binary_semaphore gui_ready{0};
    std::binary_semaphore gui_intent_to_exit{0};

    std::binary_semaphore audio_ready{0};
};
