// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stdexcept>
#include <utility>

#include "samplerate.h"

#include "clover/audio_buffer.hpp"

void convert_sample_rate_016(clover::audio_buffer& buffer, int sample_rate) {
    if (buffer.sample_rate == sample_rate) {
        return;
    }

    auto fs_out = static_cast<double>(sample_rate);
    auto fs_in  = static_cast<double>(buffer.sample_rate);
    auto ratio  = fs_out / fs_in;

    long input_frames_count  = static_cast<long>(buffer.data.size() / buffer.channels);
    long output_frames_count = static_cast<long>(static_cast<double>(input_frames_count) * ratio) + 1;

    SRC_STATE* libsamplerate_src_state = src_new(SRC_SINC_FASTEST, buffer.channels, nullptr);
    int error                          = src_error(libsamplerate_src_state);
    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    float* data_in_start = buffer.data.data();

    std::vector<float> output(output_frames_count * buffer.channels);
    float* data_out_start = &output.front();

    SRC_DATA libsampelrate_src_data = {
            .data_in       = data_in_start,
            .data_out      = data_out_start,
            .input_frames  = input_frames_count,
            .output_frames = output_frames_count,
            .src_ratio     = ratio,
    };

    error = src_process(libsamplerate_src_state, &libsampelrate_src_data);

    if (error) {
        src_delete(libsamplerate_src_state);
        throw std::runtime_error(src_strerror(error));
    }

    output.resize(libsampelrate_src_data.output_frames_gen * buffer.channels);
    src_delete(libsamplerate_src_state);

    std::swap(output, buffer.data);
    buffer.sample_rate = sample_rate;
}
