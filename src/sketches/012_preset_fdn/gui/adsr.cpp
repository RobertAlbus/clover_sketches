// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "imgui.h"
using namespace ImGui;

#include "adsr.hpp"
#include "composition/settable.hpp"

void adsr(
        std::string id,
        float* adsr,
        settable& settable_a,
        settable& settable_d,
        settable& settable_s,
        settable& settable_r) {
    float width = 30;

    ImGui::PushID(id.c_str());
    bool set_a = VSliderFloat("##A_slider", ImVec2(width, 100), adsr + 0, 1, 1000, "");
    ImGui::SameLine();
    bool set_d = VSliderFloat("##D_slider", ImVec2(width, 100), adsr + 1, 1, 50000, "");
    ImGui::SameLine();
    bool set_s = VSliderFloat("##S_slider", ImVec2(width, 100), adsr + 2, 0, 1, "");
    ImGui::SameLine();
    bool set_r = VSliderFloat("##R_slider", ImVec2(width, 100), adsr + 3, 1, 1000, "");

    ImGui::PushItemWidth(width);
    set_a = set_a || ImGui::DragFloat("##A_spinner", adsr + 0, 2, 1, 1000, "%.0f");
    ImGui::SameLine();
    set_d = set_d || ImGui::DragFloat("##D_spinner", adsr + 1, 50, 1, 50000, "%.0f");
    ImGui::SameLine();
    set_s = set_s || ImGui::DragFloat("##S_spinner", adsr + 2, 0.1f, 0, 1, "%.2f");
    ImGui::SameLine();
    set_r = set_r || ImGui::DragFloat("##R_spinner", adsr + 3, 2, 1, 1000, "%.0f");
    ImGui::PopItemWidth();

    if (set_a) {
        settable_a.set(adsr[0]);
    }
    if (set_d) {
        settable_d.set(adsr[1]);
    }
    if (set_s) {
        settable_s.set(adsr[2]);
    }
    if (set_r) {
        settable_r.set(adsr[3]);
    }

    ImGui::PopID();
}