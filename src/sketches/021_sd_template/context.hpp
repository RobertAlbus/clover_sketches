#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <semaphore>

#include "clover/io/stream.hpp"

#include "composition/graph.hpp"
#include "infrastructure/logger.hpp"
#include "sequence/sequencers.hpp"

struct context {
    context(bool render_mode = false) {
        should_loop = !render_mode;
    }
    float fs            = 48000;
    float bpm           = 160;
    float duration_bars = 4;
    bool should_loop    = true;

    int channel_count_out = 2;
    const std::string render_name{"021_sd_template"};

    logger logger;

    bar_grid grid{fs, bpm, duration_bars, should_loop};
    signal_graph graph{grid};
    sequencers sequencers{graph, grid, logger};
    clover::io::stream* stream = nullptr;

    std::binary_semaphore gui_ready{0};
    std::binary_semaphore gui_intent_to_exit{0};

    std::binary_semaphore audio_ready{0};
};