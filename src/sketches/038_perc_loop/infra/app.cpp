// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stop_token>
#include <thread>

#include "app.hpp"

void app::start() {
    std::thread audio_live([this]() { this->audio_thread(); });
    std::thread graphics_live([this]() { this->graphics_thread(); });

    std::jthread audio_render([this](std::stop_token st) { this->audio_render_thread(st); });

    audio_live.join();
    graphics_live.join();

    audio_render.request_stop();
}
