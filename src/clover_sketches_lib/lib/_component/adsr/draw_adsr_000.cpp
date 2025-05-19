// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "imgui.h"
using namespace ImGui;

#include "draw_adsr.hpp"
#include "lib/_atom/drag_slider_000.hpp"

bool draw_adsr_000(const char* id, const adsr_ranges_000& ranges, float& a, float& d, float& s, float& r) {
    ImGui::PushID(id);

    bool was_modified = false;
    if (ImGui::BeginTable("##table", 4)) {
        ImGui::TableSetupColumn("a", ImGuiTableColumnFlags_WidthFixed, 30);
        ImGui::TableSetupColumn("d", ImGuiTableColumnFlags_WidthFixed, 30);
        ImGui::TableSetupColumn("s", ImGuiTableColumnFlags_WidthFixed, 30);
        ImGui::TableSetupColumn("r", ImGuiTableColumnFlags_WidthFixed, 30);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        was_modified |= drag_slider_v_000("##a", a, 1, ranges.a_max, "%.f");
        ImGui::TableNextColumn();
        was_modified |= drag_slider_v_000("##d", d, 1, ranges.d_max, "%.f");
        ImGui::TableNextColumn();
        was_modified |= drag_slider_v_000("##s", s, 0, ranges.s_max, "%.2f");
        ImGui::TableNextColumn();
        was_modified |= drag_slider_v_000("##r", r, 1, ranges.r_max, "%.f");

        ImGui::EndTable();
    }

    ImGui::PopID();
    return was_modified;
}
