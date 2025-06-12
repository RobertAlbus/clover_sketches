// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stop_token>
#include <thread>

#include "clover/io/audio_callback.hpp"

#include "app.hpp"

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void app::start() {
    app& self = *this;

    std::thread audio_live([&self]() { self.audio_thread(); });
    std::thread graphics_live([&]() { self.graphics_thread(); });

    std::jthread audio_render([&](std::stop_token st) { self.audio_render_thread(st); });

    audio_live.join();
    graphics_live.join();

    audio_render.request_stop();
}

std::function<clover::io::callback_status(clover::io::callback_args data)> app::create_audio_callback(
        bar_grid_029& grid, signal_graph& graph, sequencers& sqs) {
    return [&](clover::io::callback_args data) {
        float& L = *(data.output);
        float& R = *(data.output + 1);

        sqs.tick();
        std::tie(L, R) = graph.tick();

        return clover::io::callback_status::cont;
    };
}
