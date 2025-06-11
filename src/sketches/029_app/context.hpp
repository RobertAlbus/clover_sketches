#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <semaphore>

#include "graph/graph.hpp"
#include "gui/view.hpp"
#include "lib/logging/logger.hpp"
#include "sequence/sequencers.hpp"

const float base_duration_bars = 32;
struct context {
    context(bool render_mode = false) {
        should_loop = !render_mode;
    }
    float fs            = 48000;
    float bpm           = 124;
    float duration_bars = 32;
    bool should_loop    = true;

    int channel_count_out = 2;
    const std::string render_name{"029_app"};

    log_bus_000 logger;

    signal_graph graph{fs};
    bar_grid grid{fs, bpm, duration_bars, should_loop};
    sequencers sequencers{graph, grid, logger};
    view view{sequencers, graph, logger};

    std::binary_semaphore gui_intent_to_exit{0};
};
