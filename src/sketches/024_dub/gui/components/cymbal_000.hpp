#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "lib/cymbal/cymbal.hpp"

#include "draw_drag_slider.hpp"

void draw_cymbal_000(cymbal_000& cymbal) {
    float available_width = ImGui::GetContentRegionAvail().x;

    for (auto& osc : cymbal.oscs) {
        float freq = osc.freq();
        ImGui::PushID(&osc);
        if (draw_drag_slider(freq, 0, 24000, available_width)) {
            osc.freq(freq);
        }
        ImGui::PopID();
    }
}
