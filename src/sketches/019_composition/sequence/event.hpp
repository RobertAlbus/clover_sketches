#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cstddef>

struct event {
    double start_time;
    double duration;
};

struct event_midi {
    double start_time;
    double duration;
    float note;
};

struct event_meta_sq {
    double start_time;
    double duration;
    size_t pattern_index;
};
