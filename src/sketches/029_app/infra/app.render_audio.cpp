// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stop_token>

#include "clover/io/audio_file.hpp"

#include "lib/render/convert_sample_rate.hpp"

#include "app.hpp"

void app::audio_render_thread(std::stop_token st) {
    std::println("starting render: {}", render_ctx->project_name());

    int render_duration_samples = render_ctx->render_duration_samples();
    int render_data_size        = render_duration_samples * render_ctx->channel_count_out();

    int runout_duration_samples = render_ctx->render_runout_duration_samples();
    int runout_data_size        = runout_duration_samples * render_ctx->channel_count_out();

    audio_buffer buffer;
    buffer.channels    = render_ctx->channel_count_out();
    buffer.sample_rate = int(render_ctx->fs());

    buffer.data.resize(render_data_size + runout_data_size, 0.f);

    auto callback = render_ctx->create_audio_callback();
    render_ctx->sequencer_start();
    for (auto frame : std::views::iota(0, render_duration_samples)) {
        if (st.stop_requested()) {
            std::println("canceled render: {}", render_ctx->project_name());
            return;
        }
        auto result = callback({
                .clock_time     = frame,
                .chan_count_in  = 0,
                .chan_count_out = render_ctx->channel_count_out(),
                .input          = nullptr,
                .output = &(buffer.data[static_cast<size_t>(frame) * render_ctx->channel_count_out()]),
        });
    }

    render_ctx->sequencer_stop();

    for (auto frame :
         std::views::iota(render_duration_samples, render_duration_samples + runout_duration_samples)) {
        if (st.stop_requested()) {
            std::println("canceled render: {}", render_ctx->project_name());
            return;
        }
        auto result = callback({
                .clock_time     = frame,
                .chan_count_in  = 0,
                .chan_count_out = render_ctx->channel_count_out(),
                .input          = nullptr,
                .output = &(buffer.data[static_cast<size_t>(frame) * render_ctx->channel_count_out()]),
        });
    }

    convert_sample_rate_016(buffer, 44100);
    clover::io::audio_file::write(
            render_ctx->project_name() + ".wav", buffer, clover::io::audio_file_settings::wav_441_16);

    clover::io::audio_file::write(
            render_ctx->project_name() + ".mp3", buffer, clover::io::audio_file_settings::mp3_320);

    std::println("finished render: {}", render_ctx->project_name());
}
