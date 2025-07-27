#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

bool resettable_knob(
    const char* label, float* value, float min, float max, float reset_to, float speed = 0, float size = 0);
