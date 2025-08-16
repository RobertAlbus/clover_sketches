// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <ranges>
#include <span>

#include "imgui.h"

#include "filter_type.hpp"

filter_type draw_filter_type_select(
    const char* id, const filter_type& current_filter_type, std::span<const enum filter_type> allowed_types) {
    filter_type new_filter_type = current_filter_type;

    const char* current_type_str = filter_str[int(current_filter_type)];
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo(id, current_type_str)) {
        for (auto [type, str] : std::views::zip(filter_types, filter_str)) {
            if (std::find(allowed_types.begin(), allowed_types.end(), type) != allowed_types.end()) {
                bool is_selected = (type == current_filter_type);
                if (ImGui::Selectable(str, is_selected, 0)) {
                    new_filter_type = type;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    return new_filter_type;
}