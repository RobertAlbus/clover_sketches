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

#include "context.hpp"
#include "infrastructure/bar_grid.hpp"

#include "composition/graph.hpp"
#include "sequence/sequencers.hpp"
#include "util.hpp"

#include "audio.hpp"

std::string render_name{"021_sd_template"};

auto create_audio_callback(bar_grid &grid, graph &comp, sequencers &sqs) {
    return [&](clover::io::callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        sqs.tick();
        std::tie(L, R) = comp.tick();

        if (!grid.should_loop && data.clock_time == size_t(grid.duration_samples()) + 1) {
            return clover::io::callback_status::end;
        }
        return clover::io::callback_status::cont;
    };
}

void AUDIO(context &ctx) {
    constexpr bool SHOULD_RENDER = false;
    if (SHOULD_RENDER) {
        std::thread render_thread = std::thread([&ctx]() {
            std::cout << "starting render: " << render_name.c_str() << std::endl;

            bar_grid grid{ctx.fs, ctx.bpm, ctx.duration_bars, false};
            graph render_comp{grid};
            sequencers render_sqs{render_comp, grid};

            auto audio_callback = create_audio_callback(grid, render_comp, render_sqs);

            audio_buffer buffer;
            buffer.channels    = render_comp.channel_count_out;
            buffer.sample_rate = int(grid.fs);
            buffer.data.resize(size_t(grid.duration_samples() * render_comp.channel_count_out), 0.f);

            for (auto frame : std::views::iota(0, int(grid.duration_samples()))) {
                auto result = audio_callback({
                        .clock_time     = frame,
                        .chan_count_in  = 0,
                        .chan_count_out = render_comp.channel_count_out,
                        .input          = nullptr,
                        .output = &(buffer.data[static_cast<size_t>(frame) * render_comp.channel_count_out]),
                });
            }

            sketch_016_convert_sample_rate(buffer, int(grid.fs));
            clover::io::audio_file::write(
                    render_name + ".wav", buffer, clover::io::audio_file_settings::wav_441_16);
            std::cout << "finished render: " << render_name.c_str() << std::endl;
        });
        render_thread.join();
        exit(0);
    }

    clover::io::system_audio_config system;
    clover::io::stream stream;

    bar_grid grid(ctx.fs, ctx.bpm, ctx.duration_bars, ctx.should_loop);
    graph comp{grid};
    sequencers sqs{comp, grid, &ctx.logger};

    ctx.graph      = &comp;
    ctx.sequencers = &sqs;

    auto audio_callback   = create_audio_callback(grid, comp, sqs);
    stream.audio_callback = audio_callback;
    // system.print();
    stream.open(clover::io::stream::settings{
            .device_index_in  = system.no_device(),
            .chan_count_in    = 0,
            .device_index_out = system.default_output().index,
            .chan_count_out   = comp.channel_count_out,
            .sample_rate      = int(grid.fs),
            .latency_ms       = 0});

    ctx.audio_ready.release();
    // props.gui_ready.acquire();

    stream.start();
    ctx.gui_intent_to_exit.acquire();
    stream.stop();
    stream.wait_to_complete();
}
