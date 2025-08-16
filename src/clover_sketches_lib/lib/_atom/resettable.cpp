// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "resettable.hpp"

bool resettable(float& value, float reset_to) {
    if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
        value = reset_to;
        return true;
    }
    return false;
}
