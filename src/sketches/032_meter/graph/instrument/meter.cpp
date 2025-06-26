// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <print>

#include "clover/math.hpp"
#include "imgui.h"

#include "lib/env_bp/env_bp.hpp"
#include "meter.hpp"

meter::meter() {
    peak_value.set_pattern(peak_hold_env);
    peak_value.duration_abs = 48000. / 60. * 10.;
    peak_value.duration_rel = 2;

    peak_hold.set_pattern(peak_hold_env);
    peak_hold.duration_abs = 48000.;
    peak_hold.duration_rel = 2;
}

void meter::tick(float x) {
    x = std::abs(x);
    if (x > peak) {
        peak_value_scale = x;
        peak_value.key_on();
    }
    if (x > peak_held) {
        peak_hold_scale = x;
        peak_hold.key_on();
    }

    peak      = peak_value.tick() * peak_value_scale;
    peak_held = peak_hold.tick() * peak_hold_scale;

    squared_ema = (alpha * x * x) + ((1 - alpha) * squared_ema);
    rms         = std::sqrt(squared_ema);
}

void draw_meter(ImVec2 dimensions, float peak, float peak_hold, float rms) {
    ImVec2 origin        = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 top_left     = origin;
    ImVec2 bottom_right = ImVec2(origin.x + dimensions.x, origin.y + dimensions.y);

    drawList->AddRectFilled(top_left, bottom_right, IM_COL32(30, 30, 30, 255));  // background

    const static float min_db = -60.0f;  // bottom of meter
    const static float max_db = 6.0f;    // top of meter

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

    const ImU32 color_peak           = ImGui::GetColorU32(ImGuiCol_FrameBg);
    const ImU32 color_peak_hold      = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
    const ImU32 color_peak_hold_clip = IM_COL32(255, 0, 0, 255);
    ;
    const ImU32 color_rms = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);

    if (peak_y != bottom_right.y) {
        drawList->AddRectFilled(ImVec2(top_left.x, peak_y), bottom_right, color_peak, 2.0f);
    }
    if (peak_hold_y != bottom_right.y) {
        drawList->AddLine(
                ImVec2(top_left.x, peak_hold_y),
                ImVec2(bottom_right.x, peak_hold_y),
                peak_hold > 1 ? color_peak_hold_clip : color_peak_hold,
                2.0f);
    }
    if (rms_y != bottom_right.y) {
        drawList->AddLine(ImVec2(top_left.x, rms_y), ImVec2(bottom_right.x, rms_y), color_rms, 2.0f);
    }

    drawList->AddLine(
            ImVec2(top_left.x, unity), ImVec2(bottom_right.x, unity), IM_COL32(255, 255, 255, 150), 1.0f);
    drawList->AddLine(
            ImVec2(top_left.x, db_m6), ImVec2(bottom_right.x, db_m6), IM_COL32(255, 255, 255, 100), 0.5f);
    drawList->AddLine(
            ImVec2(top_left.x, db_m12), ImVec2(bottom_right.x, db_m12), IM_COL32(255, 255, 255, 100), 0.5f);

    ImGui::Dummy(dimensions);
}
