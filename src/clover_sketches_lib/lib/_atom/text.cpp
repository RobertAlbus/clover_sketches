// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "imgui_internal.h"

#include "text.hpp"

void try_render_text(const char* text, float wrap_width) {
    const ImVec2 label_size     = ImGui::CalcTextSize(text, nullptr, true, wrap_width);
    const bool has_visible_text = label_size.x > 0;
    if (has_visible_text) {
        const ImVec2 label_position = ImGui::GetCursorScreenPos();
        ImGui::RenderText(label_position, text, nullptr);
        ImGui::Dummy(label_size);
    }
}