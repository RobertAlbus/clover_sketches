#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include "imgui.h"
#include "imgui_internal.h"
bool range_slider(
        const char* label,
        float* min_value,
        float* max_value,
        float min_possible,
        float max_possible,
        ImVec2 size   = ImVec2(0, 0),
        bool vertical = false) {
    if (!ImGui::GetCurrentContext())
        return false;
    bool value_changed = false;
    // allow handle cross-over
    if (*min_value > *max_value) {
        float temp = *min_value;
        *min_value = *max_value;
        *max_value = temp;
    }
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (!window)
        return false;
    ImGuiID id = window->GetID(label);
    ImGui::PushID(label);
    // Compute item size if caller didn’t specify
    ImVec2 label_size  = ImGui::CalcTextSize(label, nullptr, true);
    float frame_height = ImGui::GetFrameHeight();
    if (!vertical && size.x < 0.0f)
        size.x = ImGui::CalcItemWidth();
    if (vertical && size.y < 0.0f)
        size.y = ImGui::CalcItemWidth();
    if (!vertical && size.y <= 0.0f)
        size.y = frame_height;
    if (vertical && size.x <= 0.0f)
        size.x = frame_height;
    ImRect bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y));
    ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
    // If we're clipped or inactive, just pop and return
    if (!ImGui::ItemAdd(bb, id)) {
        ImGui::PopID();
        return false;
    }
    if (!vertical) {
        ImGui::RenderText(ImVec2(bb.Min.x, bb.Min.y + (bb.GetHeight() - label_size.y) * 0.5f), label);
    } else {
        ImGui::RenderText(ImVec2(bb.Min.x + (bb.GetWidth() - label_size.x) * 0.5f, bb.Min.y), label);
    }
    ImRect slider_bb = bb;
    if (!vertical) {
        auto min  = ImVec2(bb.Min.x + (bb.GetSize().x * 0.02f), bb.Min.y);
        auto max  = ImVec2(bb.Max.x + (bb.GetSize().x - (bb.GetSize().x * 0.02f)), bb.Max.y);
        slider_bb = ImRect(min, max);
    } else {
        auto min  = ImVec2(bb.Min.x, bb.Min.y + (bb.GetSize().y * 0.02f));
        auto max  = ImVec2(bb.Max.x, bb.Max.y - (bb.GetSize().y * 0.02f));
        slider_bb = ImRect(min, max);
    }
    float thickness     = 1.0f;
    float handle_radius = 6.0f;
    // Convert current [min_value..max_value] to [0..1] for rendering
    float range = max_possible - min_possible;
    float t_min = (*min_value - min_possible) / range;
    float t_max = (*max_value - min_possible) / range;
    t_min       = ImClamp(t_min, 0.0f, 1.0f);
    t_max       = ImClamp(t_max, 0.0f, 1.0f);
    // Compute handle positions
    auto get_handle_pos = [&](float t) {
        if (!vertical) {
            float x = ImLerp(slider_bb.Min.x, slider_bb.Max.x, t);
            float y = slider_bb.GetCenter().y;
            return ImVec2(x, y);
        }
        float x = slider_bb.GetCenter().x;
        float y = ImLerp(slider_bb.Min.y, slider_bb.Max.y, t);
        return ImVec2(x, y);
    };
    ImVec2 pos_min = get_handle_pos(t_min);
    ImVec2 pos_max = get_handle_pos(t_max);
    // Draw the slider track
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (!vertical) {
        draw_list->AddLine(
                ImVec2(slider_bb.Min.x, slider_bb.GetCenter().y),
                ImVec2(slider_bb.Max.x, slider_bb.GetCenter().y),
                IM_COL32(180, 180, 180, 255),
                thickness);
    } else {
        draw_list->AddLine(
                ImVec2(slider_bb.GetCenter().x, slider_bb.Min.y),
                ImVec2(slider_bb.GetCenter().x, slider_bb.Max.y),
                IM_COL32(180, 180, 180, 255),
                thickness);
    }
    // draw the thicker range line between handles
    draw_list->AddLine(pos_min, pos_max, IM_COL32(100, 170, 250, 255), thickness * 2.0f);
    auto handle_interaction = [&](ImVec2 handle_pos, float* value_ptr, bool is_min) {
        ImRect handle_bounding_box(
                ImVec2(handle_pos.x - handle_radius, handle_pos.y - handle_radius),
                ImVec2(handle_pos.x + handle_radius, handle_pos.y + handle_radius));
        ImGui::SetCursorScreenPos(handle_bounding_box.Min);
        ImGui::PushID(is_min ? "min_handle" : "max_handle");
        // ImGuiButtonFlags_AllowOverlap: item remains active if mouse moves off bounding box while dragging
        ImGui::InvisibleButton("##handle", handle_bounding_box.GetSize(), ImGuiButtonFlags_AllowOverlap);
        bool is_active  = ImGui::IsItemActive();
        bool is_hovered = ImGui::IsItemHovered();
        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos  = ImGui::GetIO().MousePos;
            float new_t_value = 0.0f;
            if (!vertical) {
                float fraction = (mouse_pos.x - slider_bb.Min.x) / (slider_bb.Max.x - slider_bb.Min.x);
                new_t_value    = ImClamp(fraction, 0.0f, 1.0f);
            } else {
                float fraction = (mouse_pos.y - slider_bb.Min.y) / (slider_bb.Max.y - slider_bb.Min.y);
                new_t_value    = ImClamp(fraction, 0.0f, 1.0f);
            }
            float new_value = min_possible + new_t_value * range;
            if (is_min) {
                if (new_value > *max_value)
                    new_value = *max_value;
            } else {
                if (new_value < *min_value)
                    new_value = *min_value;
            }
            if (*value_ptr != new_value) {
                *value_ptr    = new_value;
                value_changed = true;
            }
            ImGui::BeginTooltip();
            ImGui::Text("%.3f", new_value);
            ImGui::EndTooltip();
        }
        ImGui::PopID();
    };
    handle_interaction(pos_min, min_value, true);
    handle_interaction(pos_max, max_value, false);
    draw_list->AddCircleFilled(pos_min, handle_radius, IM_COL32(255, 255, 255, 255));
    draw_list->AddCircle(pos_min, handle_radius, IM_COL32(0, 0, 0, 255));
    draw_list->AddCircleFilled(pos_max, handle_radius, IM_COL32(255, 255, 255, 255));
    draw_list->AddCircle(pos_max, handle_radius, IM_COL32(0, 0, 0, 255));
    ImGui::PopID();
    // if (vertical) {
    //     ImGui::SetCursorScreenPos(ImVec2(bb.Max.x, bb.Min.y));
    // } else {
    // }
    ImGui::SetCursorScreenPos(bb.Min);
    ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
    return value_changed;
}
