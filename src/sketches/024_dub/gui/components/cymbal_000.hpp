#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "lib/cymbal/cymbal_000.hpp"

#include "lib/_atom/drag_slider_025.hpp"

void draw_cymbal_000(cymbal_000& cymbal) {
    for (auto& osc : cymbal.oscs) {
        float freq = osc.freq();
        ImGui::PushID(&osc);
        if (drag_slider_h_025(freq, 0, 24000)) {
            osc.freq(freq);
        }
        ImGui::PopID();
    }
}
