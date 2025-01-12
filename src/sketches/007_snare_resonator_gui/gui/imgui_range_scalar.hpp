#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#include "imgui_internal.h"

using namespace ImGui;

bool RangeScalar(
        const char* label,
        ImGuiDataType data_type,
        void* p_min_value,
        void* p_max_value,
        const void* p_min,
        const void* p_max,
        const char* format,
        ImGuiSliderFlags flags) {
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    // Ensure min <= max by swapping if reversed
    // (Remove this if you want to allow cross-over.)
    {
        // We'll do a small helper to swap the user data if needed.
        // We can't do a simple 'std::swap' unless we know the type is float/int, etc.
        // For simplicity, assume it's float or int. Or just read them as float for the check.
        float v_min = (data_type == ImGuiDataType_Float) ? *(float*)p_min_value : (float)*(int*)p_min_value;
        float v_max = (data_type == ImGuiDataType_Float) ? *(float*)p_max_value : (float)*(int*)p_max_value;
        if (v_min > v_max) {
            // swap bytes
            // because we don't know the size, we'll do it manually if it's float or int.
            // For brevity, let's assume float:
            // (In a real version, you'd handle each data_type carefully.)
            float temp           = *(float*)p_min_value;
            *(float*)p_min_value = *(float*)p_max_value;
            *(float*)p_max_value = temp;
        }
    }

    ImGuiContext& g         = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id        = window->GetID(label);

    // Determine widget width and bounding boxes
    const float w           = CalcItemWidth();
    const ImVec2 label_size = CalcTextSize(label, nullptr, true);
    const ImRect frame_bb(
            window->DC.CursorPos,
            window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(
            frame_bb.Min,
            frame_bb.Max +
                    ImVec2((label_size.x > 0.0f) ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format if user didn't provide
    if (!format)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    // Hover logic
    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);

    // We won't do text-input mode or nav activation in this minimal example
    // We'll just set this item active if the user clicks anywhere on the frame
    bool clicked = hovered && ImGui::IsMouseClicked(0);
    if (clicked) {
        SetActiveID(id, window);
        FocusWindow(window);
    }

    // Render frame background
    const ImU32 frame_col = GetColorU32(
            g.ActiveId == id ? ImGuiCol_FrameBgActive
            : hovered        ? ImGuiCol_FrameBgHovered
                             : ImGuiCol_FrameBg);
    RenderNavCursor(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // We'll do 2 calls to SliderBehavior:
    //   1) For the min handle
    //   2) For the max handle
    // Then we draw each handle separately.

    // For 2 handles, we need 2 unique IDs:
    // - min_id
    // - max_id
    // We can create them by "pushing" extra ID scopes or by combining with label in some unique way.
    // Here we push an extra scope for "min" or "max".

    // Make "grab_bb_min" and "grab_bb_max" to store the rectangles for each handle
    ImRect grab_bb_min, grab_bb_max;

    // 1) MIN HANDLE
    PushID("min");
    bool value_changed_min = SliderBehavior(
            frame_bb,
            GetCurrentWindow()->GetID("##min_handle"),
            data_type,
            p_min_value,
            p_min,
            p_max,
            format,
            flags,
            &grab_bb_min);
    PopID();

    // 2) MAX HANDLE
    PushID("max");
    bool value_changed_max = SliderBehavior(
            frame_bb,
            GetCurrentWindow()->GetID("##max_handle"),
            data_type,
            p_max_value,
            p_min,
            p_max,
            format,
            flags,
            &grab_bb_max);
    PopID();

    bool value_changed = (value_changed_min || value_changed_max);
    if (value_changed)
        MarkItemEdited(id);

    // Draw the two "grabs"
    // We'll reuse the same color logic as the single slider
    // The range line is drawn between the center points of each grab.
    ImVec2 center_min = ImVec2(
            (grab_bb_min.Min.x + grab_bb_min.Max.x) * 0.5f, (grab_bb_min.Min.y + grab_bb_min.Max.y) * 0.5f);
    ImVec2 center_max = ImVec2(
            (grab_bb_max.Min.x + grab_bb_max.Max.x) * 0.5f, (grab_bb_max.Min.y + grab_bb_max.Max.y) * 0.5f);

    // Draw the "range" line between min & max
    // Thicken it a bit more than a single handle
    if (center_max.x > center_min.x || center_max.y > center_min.y) {
        window->DrawList->AddLine(
                center_min, center_max, GetColorU32(ImGuiCol_SliderGrabActive), style.GrabRounding * 1.2f);
    }

    // Draw the min handle
    if (grab_bb_min.Max.x > grab_bb_min.Min.x) {
        window->DrawList->AddRectFilled(
                grab_bb_min.Min,
                grab_bb_min.Max,
                GetColorU32(
                        g.ActiveId == GetCurrentWindow()->GetID("##min_handle") ? ImGuiCol_SliderGrabActive
                                                                                : ImGuiCol_SliderGrab),
                style.GrabRounding);
    }

    // Draw the max handle
    if (grab_bb_max.Max.x > grab_bb_max.Min.x) {
        window->DrawList->AddRectFilled(
                grab_bb_max.Min,
                grab_bb_max.Max,
                GetColorU32(
                        g.ActiveId == GetCurrentWindow()->GetID("##max_handle") ? ImGuiCol_SliderGrabActive
                                                                                : ImGuiCol_SliderGrab),
                style.GrabRounding);
    }

    // Display the current range as text in the center of the bar
    // (Or you could display only one of them, or do something else.)
    // We'll just format both and display them side by side, for example:
    char value_buf[128];
    char* value_buf_end = value_buf;

    value_buf_end += DataTypeFormatString(value_buf_end, 64, data_type, p_min_value, format);
    *value_buf_end++ = ' ';
    *value_buf_end++ = '-';
    *value_buf_end++ = ' ';
    value_buf_end += DataTypeFormatString(value_buf_end, 64, data_type, p_max_value, format);

    // Render the combined text in the center
    ImVec2 text_size = CalcTextSize(value_buf, value_buf_end);
    ImVec2 text_pos =
            ImVec2((frame_bb.Min.x + frame_bb.Max.x - text_size.x) * 0.5f,
                   (frame_bb.Min.y + frame_bb.Max.y - text_size.y) * 0.5f);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, nullptr, ImVec2(0.5f, 0.5f));

    // Render the label (if any) to the right
    if (label_size.x > 0.0f) {
        RenderText(
                ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y),
                label);
    }

    // Inform test engine that we have an item here
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);

    return value_changed;
}

bool RangeFloat(
        const char* label,
        float* p_min_value,
        float* p_max_value,
        float v_min,
        float v_max,
        const char* format,
        ImGuiSliderFlags flags) {
    return RangeScalar(label, ImGuiDataType_Float, p_min_value, p_max_value, &v_min, &v_max, format, flags);
}