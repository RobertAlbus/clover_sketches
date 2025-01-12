#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.


#include "imgui.h"
#include "imgui_internal.h"

#include "imgui.h"

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

    // calculate a default size if size not provided
    ImVec2 label_size  = ImGui::CalcTextSize(label, nullptr, true);
    float frame_height = ImGui::GetFrameHeight();
    if (size.x == 0.0f && !vertical)
        size.x = ImGui::CalcItemWidth();
    if (size.y == 0.0f && vertical)
        size.y = ImGui::CalcItemWidth();
    if (!vertical && size.y == 0.0f)
        size.y = frame_height;
    if (vertical && size.x == 0.0f)
        size.x = frame_height;

    ImRect bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y));
    ImGui::ItemSize(bb, ImGui::GetStyle().FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) {
        ImGui::PopID();
        return false;
    }

    float range = max_possible - min_possible;

    // draw label to the left (or top) of the slider
    if (!vertical)
        ImGui::RenderText(ImVec2(bb.Min.x, bb.Min.y + (bb.GetHeight() - label_size.y) * 0.5f), label);
    else
        ImGui::RenderText(ImVec2(bb.Min.x + (bb.GetWidth() - label_size.x) * 0.5f, bb.Min.y), label);

    // adjust slider rect after the label
    float slider_offset = (!vertical ? label_size.x + 4.0f : label_size.y + 4.0f);
    ImRect slider_bb    = bb;
    if (!vertical) {
        slider_bb.Min.x += slider_offset;
    } else {
        slider_bb.Min.y += slider_offset;
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float thickness       = 3.0f;
    float handle_radius   = 6.0f;

    // track the state of which handle is active/hovered
    static ImGuiID active_handle_id = 0;
    static bool dragging_min_handle = false;
    static bool dragging_max_handle = false;

    // map the [min_value, max_value] to [0.0, 1.0]
    float t_min = (*min_value - min_possible) / range;
    float t_max = (*max_value - min_possible) / range;
    t_min       = ImClamp(t_min, 0.0f, 1.0f);
    t_max       = ImClamp(t_max, 0.0f, 1.0f);

    // find pixel positions for each handle
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

    // draw the full slider track
    if (!vertical)
        draw_list->AddLine(
                ImVec2(slider_bb.Min.x, slider_bb.GetCenter().y),
                ImVec2(slider_bb.Max.x, slider_bb.GetCenter().y),
                IM_COL32(180, 180, 180, 255),
                thickness);
    else
        draw_list->AddLine(
                ImVec2(slider_bb.GetCenter().x, slider_bb.Min.y),
                ImVec2(slider_bb.GetCenter().x, slider_bb.Max.y),
                IM_COL32(180, 180, 180, 255),
                thickness);

    // draw the range line between handles
    draw_list->AddLine(pos_min, pos_max, IM_COL32(100, 170, 250, 255), thickness * 2.0f);

    // helper to check interaction with a handle
    auto handle_interaction =
            [&](ImVec2 handle_pos, float* value_ptr, bool& dragging_flag, bool is_min) -> void {
        ImRect handle_rect(
                ImVec2(handle_pos.x - handle_radius, handle_pos.y - handle_radius),
                ImVec2(handle_pos.x + handle_radius, handle_pos.y + handle_radius));

        // set cursor to top-left corner of the handle region
        ImGui::SetCursorScreenPos(handle_rect.Min);

        // push an ID so min/max handle don't collide
        ImGui::PushID(is_min ? 0 : 1);
        ImGui::InvisibleButton("##handle", handle_rect.GetSize());
        ImGui::PopID();

        bool hovered = ImGui::IsItemHovered();
        bool held    = ImGui::IsItemActive();

        // start dragging
        if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            active_handle_id = ImGui::GetActiveID();
            dragging_flag    = true;
        }

        // update value if dragging
        if (dragging_flag && active_handle_id == ImGui::GetActiveID() && held) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            float new_t      = 0.0f;
            if (!vertical) {
                float fraction = (mouse_pos.x - slider_bb.Min.x) / (slider_bb.Max.x - slider_bb.Min.x);
                new_t          = ImClamp(fraction, 0.0f, 1.0f);
            } else {
                float fraction = (mouse_pos.y - slider_bb.Min.y) / (slider_bb.Max.y - slider_bb.Min.y);
                new_t          = ImClamp(fraction, 0.0f, 1.0f);
            }
            float new_value = min_possible + new_t * range;

            // keep min <= max
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

            // show tooltip when dragging
            ImGui::BeginTooltip();
            ImGui::Text("%.3f", new_value);
            ImGui::EndTooltip();
        } else if (!held && dragging_flag) {
            dragging_flag    = false;
            active_handle_id = 0;
        }
    };

    // handle interactions for both handles
    handle_interaction(pos_min, min_value, dragging_min_handle, true);
    handle_interaction(pos_max, max_value, dragging_max_handle, false);

    // draw the handles
    draw_list->AddCircleFilled(pos_min, handle_radius, IM_COL32(255, 255, 255, 255));
    draw_list->AddCircle(pos_min, handle_radius, IM_COL32(0, 0, 0, 255));

    draw_list->AddCircleFilled(pos_max, handle_radius, IM_COL32(255, 255, 255, 255));
    draw_list->AddCircle(pos_max, handle_radius, IM_COL32(0, 0, 0, 255));

    ImGui::PopID();
    return value_changed;
}
