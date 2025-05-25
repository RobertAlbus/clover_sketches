#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
using namespace ImGui;

bool drag_slider_v_000(
        const char* id,
        float& value,
        float min,
        float max,
        const char* fmt,
        ImGuiSliderFlags slider_flags = 0,
        const ImVec2& dimensions      = ImVec2(30, 100));
