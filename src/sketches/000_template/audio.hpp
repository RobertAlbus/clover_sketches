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

#include "composition/composition.hpp"
#include "sequence/sequencers.hpp"

#include "shared_props.hpp"
#include "util.hpp"

std::string render_name{"000_template"};

void AUDIO(context &props) {
    composition comp;
    sequencers sqs{comp};

    auto audio_callback = [&](clover::io::callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        sqs.tick();
        std::tie(L, R) = comp.tick();

        if (data.clock_time == comp.duration) {
            return clover::io::callback_status::end;
        }
        return clover::io::callback_status::cont;
    };

    std::jthread render_thread = std::jthread([&]() {
        std::cout << "starting render: " << render_name.c_str() << std::endl;
        composition comp;

        audio_buffer buffer;
        buffer.channels    = comp.channel_count_out;
        buffer.sample_rate = comp.fs_i;
        buffer.data.resize(comp.duration * comp.channel_count_out, 0.f);

        for (auto frame : std::views::iota(0, comp.duration)) {
            auto result = audio_callback({
                    .clock_time     = frame,
                    .chan_count_in  = 0,
                    .chan_count_out = comp.channel_count_out,
                    .input          = nullptr,
                    .output         = &(buffer.data[static_cast<size_t>(frame) * comp.channel_count_out]),
            });
        }

        sketch_016_convert_sample_rate(buffer, 44100);
        clover::io::audio_file::write(
                render_name + ".wav", buffer, clover::io::audio_file_settings::wav_441_16);
        std::cout << "finished render: " << render_name.c_str() << std::endl;
    });

    clover::io::system_audio_config system;
    clover::io::stream stream;

    props.composition = &comp;

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
