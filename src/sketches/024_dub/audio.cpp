// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>
#include <ranges>
#include <thread>

#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"

#include "lib/render/convert_sample_rate.hpp"

#include "context.hpp"
#include "lib/sq/bar_grid.hpp"

#include "graph/graph.hpp"
#include "sequence/sequencers.hpp"

#include "audio.hpp"

auto create_audio_callback(bar_grid& grid, signal_graph& comp, sequencers& sqs) {
    return [&](clover::io::callback_args data) {
        float& L = *(data.output);
        float& R = *(data.output + 1);

        sqs.tick();
        std::tie(L, R) = comp.tick();

        if (!grid.should_loop && data.clock_time == size_t(grid.duration_samples()) + 1) {
            return clover::io::callback_status::end;
        }
        return clover::io::callback_status::cont;
    };
}

void AUDIO(context& ctx) {
    std::optional<std::thread> render_thread;
    constexpr bool SHOULD_RENDER = true;

    clover::io::system_audio_config system;
    clover::io::stream stream;

    auto audio_callback   = create_audio_callback(ctx.grid, ctx.graph, ctx.sequencers);
    stream.audio_callback = audio_callback;
    // system.print();
    stream.open(
        clover::io::stream::settings{
            .device_index_in = system.no_device(),
            .chan_count_in   = 0,
            // .device_index_out = 7, // for bluetooth on laptop
            .device_index_out = system.default_output().index,  // default
            .chan_count_out   = ctx.channel_count_out,
            .sample_rate      = int(ctx.fs),
            .latency_ms       = 0});

    ctx.audio_ready.release();
    stream.start();

    if (SHOULD_RENDER) {
        render_thread = std::thread([]() {
            context render_ctx{SHOULD_RENDER};

            std::println("starting render: {}", render_ctx.render_name);

            auto audio_callback =
                create_audio_callback(render_ctx.grid, render_ctx.graph, render_ctx.sequencers);

            audio_buffer buffer;
            buffer.channels    = render_ctx.channel_count_out;
            buffer.sample_rate = int(render_ctx.grid.fs);
            buffer.data.resize(
                size_t(render_ctx.grid.duration_samples() * render_ctx.channel_count_out), 0.f);

            for (auto frame : std::views::iota(0, int(render_ctx.grid.duration_samples()))) {
                auto result = audio_callback({
                    .clock_time     = frame,
                    .chan_count_in  = 0,
                    .chan_count_out = render_ctx.channel_count_out,
                    .input          = nullptr,
                    .output = &(buffer.data[static_cast<size_t>(frame) * render_ctx.channel_count_out]),
                });
            }
            convert_sample_rate_016(buffer, 44100);
            clover::io::audio_file::write(
                render_ctx.render_name + ".wav", buffer, clover::io::audio_file_settings::wav_441_16);

            clover::io::audio_file::write(
                render_ctx.render_name + ".mp3", buffer, clover::io::audio_file_settings::mp3_320);

            std::println("finished render: {}", render_ctx.render_name);
        });
    }

    ctx.gui_intent_to_exit.acquire();
    stream.stop();
    stream.wait_to_complete();
    if (render_thread && render_thread->joinable()) {
        render_thread->join();
    }
}
