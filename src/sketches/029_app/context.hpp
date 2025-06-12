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
    float fs                   = 48000;
    float bpm                  = 124;
    double duration_bars       = 32;
    double render_repeat_count = 2;

    int channel_count_out = 2;
    const std::string render_name{"029_app"};

    log_bus_000 logger;

    signal_graph graph{fs};
    bar_grid_029 grid{fs, bpm, duration_bars};

    patterns patterns;
    arrangement arrangement;
    sequencers sequencers{build_frsq_pairs(graph, grid, logger, patterns, arrangement), grid, logger};
    view view{sequencers, graph, logger};
};
