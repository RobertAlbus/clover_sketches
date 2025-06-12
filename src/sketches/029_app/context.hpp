#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/logging/logger.hpp"
#include "lib/sq/bar_grid_029.hpp"

#include "graph/graph.hpp"
#include "gui/view.hpp"
#include "sequence/sequencers.hpp"

std::vector<frsq_pair> build_frsq_pairs(
        signal_graph& graph,
        bar_grid_029& grid,
        log_bus_000& log,
        patterns& patterns,
        arrangement& arrangement);

struct context {
    // --------------------------------
    // audio (render & live)
    float fs              = 48000;
    float bpm             = 124;
    double duration_bars  = 32;
    int channel_count_out = 2;
    patterns patterns;
    arrangement arrangement;
    bar_grid_029 grid{fs, bpm, duration_bars};
    sequencers sequencers{build_frsq_pairs(graph, grid, logger, patterns, arrangement), grid, logger};
    signal_graph graph{fs};

    // because it's integrated into sequencers (should figure out how to remove. not important though, will
    // not always use sqs logging)
    log_bus_000 logger;

    // --------------------------------
    // audio (render only)
    double render_repeat_count = 2;
    const std::string render_name{"029_app"};  // gui window name, audio render path

    // --------------------------------
    // live playback only
    const std::string window_name{"029_app"};  // gui window name, audio render path
    view view{sequencers, graph, logger};
};
