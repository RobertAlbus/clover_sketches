// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>

#include "imgui.h"

#include "clover/math.hpp"

#include "draw_meter.hpp"

void draw_meter(ImVec2 dimensions, meter_gain_mono_032& meter, bool mute) {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    draw_meter(  //
        dimensions,
        meter.peak.value,
        meter.peak_hold.value,
        meter.rms.value);
    ImGui::PopStyleVar();
}

void draw_meter(ImVec2 dimensions, meter_gain_stereo_032& meter, bool mute) {
    ImVec2 spacing = ImGui::GetStyle().ItemSpacing;

    ImVec2 origin          = ImGui::GetCursorScreenPos();
    ImVec2 half_dimensions = {dimensions.x / 2, dimensions.y};

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::BeginGroup();
    draw_meter(  //
        half_dimensions,
        meter.meter_L.peak.value,
        meter.meter_L.peak_hold.value,
        meter.meter_L.rms.value);
    ImGui::SameLine();
    draw_meter(  //
        half_dimensions,
        meter.meter_R.peak.value,
        meter.meter_R.peak_hold.value,
        meter.meter_R.rms.value);
    ImGui::EndGroup();
    ImGui::PopStyleVar();
}

void draw_meter(ImVec2 dimensions, float peak, float peak_hold, float rms, bool is_muted) {
    const ImU32 color_bg             = ImGui::GetColorU32(ImGuiCol_ChildBg);
    const ImU32 color_peak           = ImGui::GetColorU32(ImGuiCol_FrameBg);
    const ImU32 color_peak_hold      = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
    const ImU32 color_peak_hold_clip = IM_COL32(255, 0, 0, 255);
    const ImU32 color_rms            = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);
    const ImU32 color_unity          = ImGui::GetColorU32(ImGuiCol_TableBorderStrong);
    const ImU32 color_db_tick        = ImGui::GetColorU32(ImGuiCol_TableBorderLight);

    ImVec2 origin       = ImGui::GetCursorScreenPos();
    ImVec2 top_left     = origin;
    ImVec2 bottom_right = ImVec2(origin.x + dimensions.x, origin.y + dimensions.y);

    const static float min_db = -60.0f;
    const static float max_db = 6.0f;

    auto to_y = [&](float linear) {
        float db   = clover::linear_to_db(linear);
        float norm = (db - min_db) / (max_db - min_db);
        norm       = std::clamp(norm, 0.0f, 1.0f);
        return origin.y + dimensions.y * (1.0f - norm);
    };

    const float unity       = to_y(1);
    const float db_m6       = to_y(clover::db_to_linear(-6));
    const float db_m12      = to_y(clover::db_to_linear(-12));
    const float peak_y      = to_y(peak);
    const float peak_hold_y = to_y(peak_hold);
    const float rms_y       = to_y(rms);

    bool has_peak_signal      = !is_muted && peak_y != bottom_right.y;
    bool has_peak_hold_signal = !is_muted && peak_hold_y != bottom_right.y;
    bool has_rms_signal       = !is_muted && rms_y != bottom_right.y;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(top_left, bottom_right, color_bg);
    if (has_peak_signal) {
        drawList->AddRectFilled(ImVec2(top_left.x, peak_y), bottom_right, color_peak, 2.0f);
    }
    if (has_peak_hold_signal) {
        bool clip      = peak_hold >= 1;
        auto color     = clip ? color_peak_hold_clip : color_peak_hold;
        auto thickness = clip ? 3 : 2;

        drawList->AddLine(ImVec2(top_left.x, peak_hold_y), ImVec2(bottom_right.x, peak_hold_y), color, 2.0f);
    }
    if (has_rms_signal) {
        drawList->AddLine(ImVec2(top_left.x, rms_y), ImVec2(bottom_right.x, rms_y), color_rms, 2.0f);
    }

    drawList->AddLine(ImVec2(top_left.x, unity), ImVec2(bottom_right.x, unity), color_unity, 1.0f);
    drawList->AddLine(ImVec2(top_left.x, db_m6), ImVec2(bottom_right.x, db_m6), color_db_tick, 0.5f);
    drawList->AddLine(ImVec2(top_left.x, db_m12), ImVec2(bottom_right.x, db_m12), color_db_tick, 0.5f);

    ImGui::Dummy(dimensions);
}
