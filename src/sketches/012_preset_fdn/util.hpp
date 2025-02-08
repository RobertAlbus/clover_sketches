#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <stdexcept>

#include "samplerate.h"

#include "clover/audio_buffer.hpp"

using namespace clover;

void sketch_008_convert_sample_rate(audio_buffer& buffer, int sample_rate) {
    if (buffer.sample_rate == sample_rate) {
        return;
    }

    SRC_STATE* libsamplerate_src_state = src_new(SRC_SINC_FASTEST, buffer.channels, nullptr);

    int error = src_error(libsamplerate_src_state);
    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    float* data_in_start     = buffer.data.data();
    long input_frames_count  = static_cast<long>(buffer.data.size() / buffer.channels);
    double ratio             = static_cast<float>(sample_rate) / static_cast<float>(buffer.sample_rate);
    long output_frames_count = static_cast<long>(static_cast<double>(input_frames_count) * ratio) + 1;

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
    src_delete(libsamplerate_src_state);
    output.resize(libsampelrate_src_data.output_frames_gen * buffer.channels);

    buffer.data = std::move(output);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }
}