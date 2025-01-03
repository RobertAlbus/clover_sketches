// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "util/audio_buffer_normalize.hpp"

using namespace clover;

namespace sketch {

void normalize_buffer(audio_buffer& buffer) {
    clover_float max_value = 0;
    for (auto sample : buffer.data)
        max_value = std::max(max_value, sample);

    max_value *= 0.8;

    for (auto& sample : buffer.data)
        sample *= max_value;
}

}  // namespace sketch