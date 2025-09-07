#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

struct adsr_values {
    float a;
    float d;
    float s;
    float r;

    explicit operator std::string() const;
};
