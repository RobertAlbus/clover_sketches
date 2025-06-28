#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "meter.hpp"

void draw_meter(ImVec2 dimensions, float peak, float peak_hold, float rms);

void draw_meter(ImVec2 dimensions, meter_gain_mono_032& meter);
void draw_meter(ImVec2 dimensions, meter_gain_stereo_032& meter);
