// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui-knobs.h"
#include "imgui.h"

#include "knob.hpp"
#include "text.hpp"

bool resettable_knob(
    const char* label, float* value, float min, float max, float reset_to, float speed, float size) {
    const float original_value = *value;
    ImGui::PushID(label);

    try_render_text(label);

    ImGuiKnobs::Knob("", value, min, max, 0, "%3.2f", ImGuiKnobVariant_Tick, size, 0);

    if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
        *value = reset_to;
    }

    ImGui::PopID();
    return *value != original_value;
};
