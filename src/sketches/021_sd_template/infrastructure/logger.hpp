#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "concurrentqueue.h"

struct gui_log_message {
    char text[256];
};

struct logger {
    moodycamel::ConcurrentQueue<gui_log_message> gui;
};
