#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "concurrentqueue.h"

struct log_message_000 {
    char text[256];
};

struct log_bus_000 {
    moodycamel::ConcurrentQueue<log_message_000> gui;
};
