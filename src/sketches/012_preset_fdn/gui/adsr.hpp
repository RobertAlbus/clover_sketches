#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "composition/settable.hpp"

void adsr(
        std::string id,
        float* adsr,
        settable& settable_a,
        settable& settable_d,
        settable& settable_s,
        settable& settable_r);