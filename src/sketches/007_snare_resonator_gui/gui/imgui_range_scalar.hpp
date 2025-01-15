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

    ImGuiContext& g         = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id        = window->GetID(label);
    const bool is_vertical  = (flags & ImGuiSliderFlags_Vertical) != 0;

    const float w           = CalcItemWidth();
    const ImVec2 label_size = CalcTextSize(label, nullptr, true);
    const ImRect frame_bb =
            is_vertical
                    ? ImRect(window->DC.CursorPos,
                             window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2.0f, w))
                    : ImRect(window->DC.CursorPos,
                             window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb =
            is_vertical ? ImRect(frame_bb.Min,
                                 frame_bb.Max + ImVec2(0.0f,
                                                       (label_size.x > 0.0f)
                                                               ? (style.ItemInnerSpacing.x + label_size.x)
                                                               : 0.0f))
                        : ImRect(frame_bb.Min,
                                 frame_bb.Max + ImVec2((label_size.x > 0.0f)
                                                               ? (style.ItemInnerSpacing.x + label_size.x)
                                                               : 0.0f,
                                                       0.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format if user didn't provide
    if (!format)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    // Render frame background
    const ImU32 frame_col = GetColorU32(ImGuiCol_FrameBg);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Create IDs for the min and max handles with unique scope
    PushID(label);
    const ImGuiID min_id = GetID("##min_handle");
    const ImGuiID max_id = GetID("##max_handle");
    PopID();

    auto apply_logarithmic = [&](float t, const void* min, const void* max) -> float {
        if ((flags & ImGuiSliderFlags_Logarithmic) == 0)
            return t;
        float log_min = log10f(*(float*)min + FLT_EPSILON);
        float log_max = log10f(*(float*)max + FLT_EPSILON);
        return log10f(*(float*)p_min_value + FLT_EPSILON) / (log_max - log_min);
    };
    float t_min = apply_logarithmic(
            (*(float*)p_min_value - *(float*)p_min) / (*(float*)p_max - *(float*)p_min), p_min, p_max);
    float t_max = apply_logarithmic(
            (*(float*)p_max_value - *(float*)p_min) / (*(float*)p_max - *(float*)p_min), p_min, p_max);
    auto get_grab_bb = [&](float t) -> ImRect {
        if (is_vertical) {
            float pos = ImLerp(frame_bb.Max.y, frame_bb.Min.y, t);
            return {ImVec2(frame_bb.Min.x, pos - style.GrabMinSize * 0.5f),
                    ImVec2(frame_bb.Max.x, pos + style.GrabMinSize * 0.5f)};
        }
        float pos = ImLerp(frame_bb.Min.x, frame_bb.Max.x, t);
        return {ImVec2(pos - style.GrabMinSize * 0.5f, frame_bb.Min.y),
                ImVec2(pos + style.GrabMinSize * 0.5f, frame_bb.Max.y)};
    };
    ImRect grab_bb_min     = get_grab_bb(t_min);
    ImRect grab_bb_max     = get_grab_bb(t_max);
    bool value_changed_min = false, value_changed_max = false;
    auto handle_interaction = [&](ImGuiID handle_id, ImRect& grab_bb, float* value_ptr, bool is_min) {
        if (ItemAdd(grab_bb, handle_id)) {
            if (IsMouseHoveringRect(grab_bb.Min, grab_bb.Max) && IsMouseClicked(0)) {
                SetActiveID(handle_id, window);
                BeginTooltip();
                Text(format, *(float*)value_ptr);
                EndTooltip();
            }
            if (g.ActiveId == handle_id) {
                if (IsMouseDragging(0)) {
                    float mouse_t = is_vertical ? ImSaturate(
                                                          (GetIO().MousePos.y - frame_bb.Max.y) /
                                                          (frame_bb.Min.y - frame_bb.Max.y))
                                                : ImSaturate(
                                                          (GetIO().MousePos.x - frame_bb.Min.x) /
                                                          (frame_bb.Max.x - frame_bb.Min.x));
                    if (flags & ImGuiSliderFlags_Logarithmic) {
                        float log_min = log10f(*(float*)p_min + FLT_EPSILON);
                        float log_max = log10f(*(float*)p_max + FLT_EPSILON);
                        *value_ptr    = powf(10.0f, log_min + mouse_t * (log_max - log_min));
                    } else {
                        *value_ptr = ImLerp(*(float*)p_min, *(float*)p_max, mouse_t);
                    }
                    *value_ptr = ImClamp(*value_ptr, *(float*)p_min, *(float*)p_max);
                    if (is_min && *value_ptr > *(float*)p_max_value) {
                        ImSwap(*value_ptr, *(float*)p_max_value);
                        SetActiveID(max_id, window);
                    } else if (!is_min && *value_ptr < *(float*)p_min_value) {
                        ImSwap(*value_ptr, *(float*)p_min_value);
                        SetActiveID(min_id, window);
                    }
                }
                if (!IsMouseDown(0))
                    ClearActiveID();
                return true;
            }
        }
        return false;
    };
    value_changed_min = handle_interaction(min_id, grab_bb_min, (float*)p_min_value, true);
    value_changed_max = handle_interaction(max_id, grab_bb_max, (float*)p_max_value, false);
    ImVec2 center_min = is_vertical
                                ? ImVec2((grab_bb_min.Min.x + grab_bb_min.Max.x) * 0.5f, grab_bb_min.Min.y)
                                : ImVec2(grab_bb_min.GetCenter());
    ImVec2 center_max = is_vertical
                                ? ImVec2((grab_bb_max.Min.x + grab_bb_max.Max.x) * 0.5f, grab_bb_max.Min.y)
                                : ImVec2(grab_bb_max.GetCenter());
    window->DrawList->AddLine(center_min, center_max, GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
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

    char value_buf_min[64], value_buf_max[64];

    DataTypeFormatString(value_buf_min, sizeof(value_buf_min), data_type, p_min_value, format);
    DataTypeFormatString(value_buf_max, sizeof(value_buf_max), data_type, p_max_value, format);

    char value_buf[128];

    if (is_vertical) {
        snprintf(value_buf, sizeof(value_buf), "%s\n -\n%s", value_buf_max, value_buf_min);
        RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, nullptr, nullptr, ImVec2(0.5f, 0.5f));
    } else {
        snprintf(value_buf, sizeof(value_buf), "%s - %s", value_buf_min, value_buf_max);
        RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, nullptr, nullptr, ImVec2(0.5f, 0.5f));
    }

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
