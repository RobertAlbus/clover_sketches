#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/audio_frame/audio_frame.hpp"

struct pan_033 {
    audio_frame_stereo pan_coeffs;

    void set(float pan);
    float get() const;

    audio_frame_stereo tick(audio_frame_stereo x) const;
};
