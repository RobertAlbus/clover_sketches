#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

void draw_meter(ImVec2 dimensions, float peak, float peak_hold, float rms);
