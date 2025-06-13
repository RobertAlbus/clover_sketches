#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <memory>
#include <semaphore>
#include <stop_token>

#include "context.hpp"

struct app {
    template <typename context_t>
    static app create() {
        static_assert(
                std::is_base_of<context_base, context_t>::value, "context_t must derive from context_base");
        return app{std::make_unique<context_t>(), std::make_unique<context_t>()};
    }

    app(std::unique_ptr<context_base> live, std::unique_ptr<context_base> render)
        : live_ctx{std::move(live)}, render_ctx{std::move(render)} {
    }

    std::unique_ptr<context_base> live_ctx;
    std::unique_ptr<context_base> render_ctx;

    void start();
    void audio_thread();
    void graphics_thread();
    void audio_render_thread(std::stop_token st);

    std::binary_semaphore gui_intent_to_exit{0};
};
