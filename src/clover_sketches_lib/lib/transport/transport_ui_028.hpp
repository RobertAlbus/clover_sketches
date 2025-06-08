#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>

struct transport_ui_028 {
    transport_ui_028(std::function<void(float)> play_action, std::function<void()> stop_action);
    float bar       = 0;
    bool is_playing = false;

    std::function<void(float)> play_action;
    std::function<void()> stop_action;
    void draw();
};
