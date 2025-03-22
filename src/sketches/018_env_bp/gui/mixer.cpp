// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "imgui.h"
using namespace ImGui;

#include "mixer.hpp"

void mixer(float* loop, float* verb_in, float* verb_out) {
    float width = 30;

    ImGui::PushID("mixer");
    bool set_a = VSliderFloat("dry##loop_slider", ImVec2(width, 100), loop, 0, 2, "");
    ImGui::SameLine();
    bool set_d = VSliderFloat("in##verb_in_slider", ImVec2(width, 100), verb_in, 0, 2, "");
    ImGui::SameLine();
    bool set_s = VSliderFloat("out##verb_out_slider", ImVec2(width, 100), verb_out, 0, 2, "");

    ImGui::PushItemWidth(width);
    set_a = set_a || ImGui::DragFloat("##loop_spinner", loop, 0.1f, 0, 2, "%.2f");
    ImGui::SameLine();
    set_d = set_d || ImGui::DragFloat("##verb_in_spinner", verb_in, 0.1f, 0, 2, "%.2f");
    ImGui::SameLine();
    set_s = set_s || ImGui::DragFloat("##verb_out_spinner", verb_out, 0.1f, 0, 2, "%.2f");

    ImGui::PopItemWidth();
    ImGui::PopID();
}
