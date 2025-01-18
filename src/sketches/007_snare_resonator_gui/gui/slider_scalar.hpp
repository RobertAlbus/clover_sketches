#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#include "imgui_internal.h"

bool ImGui::SliderScalar(
        const char* label,
        ImGuiDataType data_type,
        void* p_data,
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
    const float w           = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, nullptr, true);
    const ImRect frame_bb(
            window->DC.CursorPos,
            window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(
            frame_bb.Min,
            frame_bb.Max +
                    ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == nullptr)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered        = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active) {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool clicked     = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
        const bool make_active = (clicked || g.NavActivateId == id);
        if (make_active && clicked)
            SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl) ||
                (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active) {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active) {
        // Only clamp CTRL+Click input when ImGuiSliderFlags_ClampOnInput is set (generally via
        // ImGuiSliderFlags_AlwaysClamp)
        const bool clamp_enabled = (flags & ImGuiSliderFlags_ClampOnInput) != 0;
        return TempInputScalar(
                frame_bb,
                id,
                label,
                data_type,
                p_data,
                format,
                clamp_enabled ? p_min : nullptr,
                clamp_enabled ? p_max : nullptr);
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(
            g.ActiveId == id ? ImGuiCol_FrameBgActive
            : hovered        ? ImGuiCol_FrameBgHovered
                             : ImGuiCol_FrameBg);
    RenderNavCursor(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed =
            SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(
                grab_bb.Min,
                grab_bb.Max,
                GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab),
                style.GrabRounding);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the
    // value.
    char value_buf[64];
    const char* value_buf_end =
            value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    if (g.LogEnabled)
        LogSetNextTextDecoration("{", "}");
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, nullptr, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(
                ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y),
                label);

    IMGUI_TEST_ENGINE_ITEM_INFO(
            id,
            label,
            g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}