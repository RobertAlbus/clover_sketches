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
    float fs            = 48000;
    float bpm           = 160;
    float duration_bars = 16;
    bool should_loop    = true;

    graph* graph               = nullptr;
    sequencers* sequencers     = nullptr;
    clover::io::stream* stream = nullptr;

    logger logger;

    std::binary_semaphore gui_ready{0};
    std::binary_semaphore gui_intent_to_exit{0};

    std::binary_semaphore audio_ready{0};
};