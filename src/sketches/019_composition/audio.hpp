#pragma once

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

#include "composition/composition.hpp"
#include "sequence/sequencers.hpp"

#include "shared_props.hpp"

std::string render_name{"019_composition"};

auto create_audio_callback(composition& comp, sequencers& sqs) {
    return [&](clover::io::callback_args data) {
        float& L = *(data.output);
        float& R = *(data.output + 1);

        sqs.tick();
        std::tie(L, R) = comp.tick();

        if (data.clock_time == comp.duration) {
            return clover::io::callback_status::end;
        }
        return clover::io::callback_status::cont;
    };
}

void AUDIO(context& props) {
    composition comp;
    sequencers sqs{comp};

    auto audio_callback = create_audio_callback(comp, sqs);

    constexpr bool SHOULD_RENDER = false;
    if (SHOULD_RENDER) {
        std::thread render_thread = std::thread([]() {
            std::cout << "starting render: " << render_name.c_str() << std::endl;

            composition render_comp;
            sequencers render_sqs{render_comp};

            auto audio_callback = create_audio_callback(render_comp, render_sqs);

            audio_buffer buffer;
            buffer.channels    = render_comp.channel_count_out;
            buffer.sample_rate = render_comp.fs_i;
            buffer.data.resize(render_comp.duration * render_comp.channel_count_out, 0.f);

            for (auto frame : std::views::iota(0, render_comp.duration)) {
                auto result = audio_callback({
                        .clock_time     = frame,
                        .chan_count_in  = 0,
                        .chan_count_out = render_comp.channel_count_out,
                        .input          = nullptr,
                        .output = &(buffer.data[static_cast<size_t>(frame) * render_comp.channel_count_out]),
                });
            }

            convert_sample_rate_016(buffer, 44100);
            clover::io::audio_file::write(
                    render_name + ".wav", buffer, clover::io::audio_file_settings::wav_441_16);
            std::cout << "finished render: " << render_name.c_str() << std::endl;
        });
        render_thread.join();
        exit(0);
    }

    clover::io::system_audio_config system;
    clover::io::stream stream;

    props.composition = &comp;
    props.sequencers  = &sqs;

    props.audio_ready.release();
    props.gui_ready.acquire();

    stream.audio_callback = audio_callback;
    // system.print();
    stream.open(clover::io::stream::settings{
            .device_index_in  = system.no_device(),
            .chan_count_in    = 0,
            .device_index_out = system.default_output().index,
            .chan_count_out   = comp.channel_count_out,
            .sample_rate      = comp.fs_i,
            .latency_ms       = 0});

    stream.start();
    props.gui_intent_to_exit.acquire();
    stream.stop();
    stream.wait_to_complete();
}
