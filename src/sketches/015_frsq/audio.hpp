#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>
#include <thread>

#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"

#include "composition.hpp"
#include "shared_props.hpp"
#include "util.hpp"

std::string render_name{"015_frsq.wav"};

void AUDIO(context& props) {
    std::jthread render_thread = std::jthread([]() {
        std::cout << "starting render: " << render_name.c_str() << std::endl;
        composition comp;
        audio_buffer rendered_audio =
                exec_callback(comp.audio_callback, comp.channel_count_out, comp.fs_i, comp.duration);
        sketch_008_convert_sample_rate(rendered_audio, 44100);
        audio_file::write(render_name, rendered_audio, audio_file_settings::wav_441_16);
        std::cout << "finished render: " << render_name.c_str() << std::endl;
    });

    composition comp;
    system_audio_config system;

    stream stream;

    props.composition = &comp;

    props.audio_ready.release();
    props.gui_ready.acquire();

    stream.audio_callback = comp.audio_callback;

    // system.print();
    stream.open(stream::settings{
            .device_index_in  = system.no_device(),
            .chan_count_in    = 0,
            .device_index_out = system.default_output().index,  // 7,  // output: "pulse" with pulse bridged
                                                                // to JACK so I can use bluetooth
            .chan_count_out = comp.channel_count_out,
            .sample_rate    = comp.fs_i,
            .latency_ms     = 0});

    stream.start();
    props.gui_intent_to_exit.acquire();
    stream.stop();
    stream.wait_to_complete();

    if (render_thread.joinable()) {
        std::cout << "check for render complete." << std::endl;
        render_thread.join();
        std::cout << "render complete." << std::endl;
    }
}