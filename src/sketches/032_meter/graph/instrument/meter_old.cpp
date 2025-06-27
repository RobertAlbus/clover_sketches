// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "clover/math.hpp"
#include "imgui.h"

#include "lib/env_bp/env_bp.hpp"
#include "meter_old.hpp"

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

    bool has_peak_signal      = peak_y != bottom_right.y;
    bool has_peak_hold_signal = peak_hold_y != bottom_right.y;
    bool has_rms_signal       = rms_y != bottom_right.y;

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
