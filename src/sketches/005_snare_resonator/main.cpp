// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <thread>

#include "clover/audio_buffer.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"

#include "composition.hpp"

bool PLAYBACK = true;
bool RENDER   = true;

std::string render_name{"05_snare_resonator.wav"};

using namespace clover;
using namespace io;

int main(int argc, char *argv[]) {
    std::thread render_thread;
    if (RENDER) {
        render_thread = std::thread([]() {
            std::cout << "render started. " << std::endl;
            composition comp;
            audio_buffer rendered_audio =
                    exec_callback(comp.audio_callback, comp.channel_count_out, comp.fs_i, comp.duration);
            normalize_audio_buffer(rendered_audio);
            audio_file::write(render_name, rendered_audio, audio_file_settings::wav_441_16);
            std::cout << "render complete. " << std::endl;
        });
    }

    if (PLAYBACK) {
        composition comp;

        system_audio_config system;
        stream stream;
        stream.audio_callback = comp.audio_callback;

        stream.open(stream::settings{
                .device_index_in  = system.no_device(),
                .chan_count_in    = 0,
                .device_index_out = system.default_output().index,
                .chan_count_out   = comp.channel_count_out,
                .sample_rate      = comp.fs_i,
                .latency_ms       = 0});

        stream.start();
        stream.wait_to_complete();
    }
    if (render_thread.joinable()) {
        std::cout << "joining render_thread." << std::endl;
        render_thread.join();
    }
    std::cout << "done." << std::endl;
}
