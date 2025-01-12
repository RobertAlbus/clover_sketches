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
    {
        float v_min = (data_type == ImGuiDataType_Float) ? *(float*)p_min_value : (float)*(int*)p_min_value;
        float v_max = (data_type == ImGuiDataType_Float) ? *(float*)p_max_value : (float)*(int*)p_max_value;
        if (v_min > v_max) {
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

    // Render frame background
    const ImU32 frame_col = GetColorU32(ImGuiCol_FrameBg);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Create IDs for the min and max handles
    const ImGuiID min_id = window->GetID("##min_handle");
    const ImGuiID max_id = window->GetID("##max_handle");

    // Normalized positions for the handles
    float t_min = (*(float*)p_min_value - *(float*)p_min) / (*(float*)p_max - *(float*)p_min);
    float t_max = (*(float*)p_max_value - *(float*)p_min) / (*(float*)p_max - *(float*)p_min);

    // Compute grab positions
    ImRect grab_bb_min(
            ImVec2(ImLerp(frame_bb.Min.x, frame_bb.Max.x, t_min) - style.GrabMinSize * 0.5f, frame_bb.Min.y),
            ImVec2(ImLerp(frame_bb.Min.x, frame_bb.Max.x, t_min) + style.GrabMinSize * 0.5f, frame_bb.Max.y));
    ImRect grab_bb_max(
            ImVec2(ImLerp(frame_bb.Min.x, frame_bb.Max.x, t_max) - style.GrabMinSize * 0.5f, frame_bb.Min.y),
            ImVec2(ImLerp(frame_bb.Min.x, frame_bb.Max.x, t_max) + style.GrabMinSize * 0.5f, frame_bb.Max.y));

    // Handle interactions for the min handle
    bool value_changed_min = false;
    bool value_changed_max = false;

    if (ItemAdd(grab_bb_min, min_id) && IsMouseHoveringRect(grab_bb_min.Min, grab_bb_min.Max)) {
        if (IsMouseClicked(0)) {
            SetActiveID(min_id, window);
        }
        if (IsItemActive() && IsMouseDragging(0)) {
            float mouse_t =
                    ImSaturate((GetIO().MousePos.x - frame_bb.Min.x) / (frame_bb.Max.x - frame_bb.Min.x));
            *(float*)p_min_value = ImLerp(*(float*)p_min, *(float*)p_max, mouse_t);
            *(float*)p_min_value = ImClamp(*(float*)p_min_value, *(float*)p_min, *(float*)p_max);
            value_changed_min    = true;
        }
    }

    // Handle interactions for the max handle
    if (ItemAdd(grab_bb_max, max_id) && IsMouseHoveringRect(grab_bb_max.Min, grab_bb_max.Max)) {
        if (IsMouseClicked(0)) {
            SetActiveID(max_id, window);
        }
        if (IsItemActive() && IsMouseDragging(0)) {
            float mouse_t =
                    ImSaturate((GetIO().MousePos.x - frame_bb.Min.x) / (frame_bb.Max.x - frame_bb.Min.x));
            *(float*)p_max_value = ImLerp(*(float*)p_min, *(float*)p_max, mouse_t);
            *(float*)p_max_value = ImClamp(*(float*)p_max_value, *(float*)p_min, *(float*)p_max);
            value_changed_max    = true;
        }
    }

    // Draw the range line
    ImVec2 center_min = ImVec2(
            (grab_bb_min.Min.x + grab_bb_min.Max.x) * 0.5f, (grab_bb_min.Min.y + grab_bb_min.Max.y) * 0.5f);
    ImVec2 center_max = ImVec2(
            (grab_bb_max.Min.x + grab_bb_max.Max.x) * 0.5f, (grab_bb_max.Min.y + grab_bb_max.Max.y) * 0.5f);
    window->DrawList->AddLine(center_min, center_max, GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);

    // Draw the handles
    window->DrawList->AddRectFilled(
            grab_bb_min.Min,
            grab_bb_min.Max,
            GetColorU32(g.ActiveId == min_id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab),
            style.GrabRounding);
    window->DrawList->AddRectFilled(
            grab_bb_max.Min,
            grab_bb_max.Max,
            GetColorU32(g.ActiveId == max_id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab),
            style.GrabRounding);

    // Display the current range as text
    char value_buf[128];
    snprintf(value_buf, sizeof(value_buf), format, *(float*)p_min_value, *(float*)p_max_value);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, nullptr, nullptr, ImVec2(0.5f, 0.5f));

    return value_changed_min || value_changed_max;
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
