#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <semaphore>

#include "clover/io/stream.hpp"
#include "composition.hpp"

struct shared_props {
    composition* composition   = nullptr;
    clover::io::stream* stream = nullptr;

    std::binary_semaphore gui_ready{0};
    std::binary_semaphore gui_intent_to_exit{0};

    std::binary_semaphore audio_ready{0};
};