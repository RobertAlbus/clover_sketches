// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/oscillator.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/io/stream.hpp"
#include "clover/io/system_audio.hpp"
#include "clover/math.hpp"

bool PLAYBACK = false;
bool RENDER   = true;

using namespace clover;
using namespace dsp;
using namespace io;

int main(int argc, char *argv[]) {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 5 * fs_i;
    int channel_count_out = 2;

    oscillator osc(fs);
    oscillator mod(fs);

    auto audio_state_init = [&]() {
        osc.freq(400);
        mod.freq(2);
        osc.phase(0);
        mod.phase(0);
    };

    auto audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        L = osc.tick();
        R = L;

        float freq = frequency_by_octave_difference(400, (7.f / 12.f) * mod.tick());
        osc.freq(freq);

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };

    if (PLAYBACK) {
        system_audio_config system;
        system.print();
        stream stream;
        stream.audio_callback = audio_callback;

        audio_state_init();
        stream.open(stream::settings{
                .device_index_in  = system.no_device(),
                .chan_count_in    = 0,
                .device_index_out = system.default_output().index,
                .chan_count_out   = channel_count_out,
                .sample_rate      = fs_i,
                .latency_ms       = 0});

        stream.start();
        stream.wait_to_complete();
        stream.stop();
    }

    if (RENDER) {
        audio_state_init();
        audio_buffer render = exec_callback(audio_callback, channel_count_out, fs_i, duration);
        audio_file::write("test.wav", render, audio_file_settings::wav_441_16);
    }
}
