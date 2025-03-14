// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui_internal.h"

#include "clover/math.hpp"

#include "audio_meter_poc.hpp"

void audio_meter(const char* id, float level, ImVec2 size) {
    float floorDB           = -40.0f;
    float yellowThresholdDB = -6.0f;
    ImDrawList* draw_list   = ImGui::GetWindowDrawList();
    ImVec2 pos              = ImGui::GetCursorScreenPos();
    const ImU32 bgColor     = IM_COL32(50, 50, 50, 255);
    const ImU32 greenColor  = IM_COL32(0, 200, 0, 255);
    const ImU32 yellowColor = IM_COL32(200, 200, 0, 255);
    const ImU32 redColor    = IM_COL32(200, 0, 0, 255);
    const ImU32 borderColor = IM_COL32(255, 255, 255, 255);

    float clipAreaHeight = size.y * 0.2f;
    float mainAreaHeight = size.y - clipAreaHeight;
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor);

    ImVec2 mainPos    = ImVec2(pos.x, pos.y + clipAreaHeight);
    ImVec2 mainBottom = ImVec2(pos.x + size.x, pos.y + size.y);
    float mainLevel   = (level > 1.0f) ? 1.0f : level;
    float currentDB   = (mainLevel > 0.0f) ? clover::linear_to_db(mainLevel) : floorDB;
    if (mainLevel <= 0.0f)
        currentDB = floorDB;
    float normalizedFill            = (currentDB - floorDB) / (0.0f - floorDB);
    normalizedFill                  = ImClamp(normalizedFill, 0.0f, 1.0f);
    float fillHeight                = mainAreaHeight * normalizedFill;
    float yellowThresholdNormalized = (yellowThresholdDB - floorDB) / (0.0f - floorDB);
    float greenFillHeight           = mainAreaHeight * ImMin(normalizedFill, yellowThresholdNormalized);
    float yellowFillHeight          = (normalizedFill > yellowThresholdNormalized)
                                              ? mainAreaHeight * (normalizedFill - yellowThresholdNormalized)
                                              : 0.0f;

    if (greenFillHeight > 0.0f)
        draw_list->AddRectFilled(
                ImVec2(mainPos.x, mainPos.y + mainAreaHeight - greenFillHeight),
                ImVec2(mainPos.x + size.x, mainPos.y + mainAreaHeight),
                greenColor);
    if (yellowFillHeight > 0.0f)
        draw_list->AddRectFilled(
                ImVec2(mainPos.x, mainPos.y + mainAreaHeight - fillHeight),
                ImVec2(mainPos.x + size.x, mainPos.y + mainAreaHeight - greenFillHeight),
                yellowColor);

    if (level > 1.0f) {
        float clampedLevel   = ImClamp(level, 1.0f, 2.0f);
        float clipDB         = clover::linear_to_db(clampedLevel);
        float clipNormalized = clipDB / clover::linear_to_db(2.0f);
        clipNormalized       = ImClamp(clipNormalized, 0.0f, 1.0f);
        float clipFillHeight = clipAreaHeight * clipNormalized;
        draw_list->AddRectFilled(
                ImVec2(pos.x, pos.y + clipAreaHeight - clipFillHeight),
                ImVec2(pos.x + size.x, pos.y + clipAreaHeight),
                redColor);
    }
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), borderColor);
    ImGui::Dummy(size);
}