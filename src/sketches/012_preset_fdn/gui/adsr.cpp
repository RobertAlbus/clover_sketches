// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "imgui.h"
using namespace ImGui;

#include "adsr.hpp"
#include "composition/settable.hpp"

bool slider_spinnner_v(
        const char* id,
        settable& settable_param,
        float min,
        float max,
        const char* fmt,
        ImGuiSliderFlags slider_flags,
        const ImVec2& dimensions) {
    ImGui::PushID(id);
    slider_flags |= ImGuiSliderFlags_NoRoundToFormat;

    bool was_modified =
            VSliderFloat("##slider", dimensions, &settable_param.gui, min, max, fmt, slider_flags);

    ImGui::PushItemWidth(dimensions.x);
    was_modified =
            was_modified || ImGui::DragFloat("##spinner", &settable_param.gui, max * 0.2f, min, max, fmt);
    ImGui::PopItemWidth();

    ImGui::PopID();

    if (was_modified) {
        settable_param.set(settable_param.gui);
    }

    return was_modified;
}

bool slider_spinnner_v(
        const char* id,
        settable& settable_param,
        float min,
        float max,
        const char* fmt,
        ImGuiSliderFlags slider_flags) {
    const ImVec2 default_dimensions = ImVec2(30, 100);
    return slider_spinnner_v(id, settable_param, min, max, fmt, slider_flags, default_dimensions);
}

bool adsr(
        const char* id,
        const adsr_ranges& ranges,
        settable& settable_a,
        settable& settable_d,
        settable& settable_s,
        settable& settable_r) {
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
        was_modified = was_modified || slider_spinnner_v("##a", settable_a, 1, ranges.a_max, "%.f");
        ImGui::TableNextColumn();
        was_modified = was_modified || slider_spinnner_v("##d", settable_d, 1, ranges.d_max, "%.f");
        ImGui::TableNextColumn();
        was_modified = was_modified || slider_spinnner_v("##s", settable_s, 0, ranges.s_max, "%.2f");
        ImGui::TableNextColumn();
        was_modified = was_modified || slider_spinnner_v("##r", settable_r, 1, ranges.r_max, "%.f");
        ImGui::EndTable();
    }

    ImGui::PopID();
    return was_modified;
}