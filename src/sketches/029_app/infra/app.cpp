// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stop_token>
#include <thread>

#include "app.hpp"

void app::start() {
    app& self = *this;

    std::thread audio_live([&self]() { self.audio_thread(); });
    std::thread graphics_live([&]() { self.graphics_thread(); });

    std::jthread audio_render([&](std::stop_token st) { self.audio_render_thread(st); });

    audio_live.join();
    graphics_live.join();

    audio_render.request_stop();
}
