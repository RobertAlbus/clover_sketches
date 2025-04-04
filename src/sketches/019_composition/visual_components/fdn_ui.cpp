// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <string>

#include "imgui.h"
using namespace ImGui;

#include "fdn_ui.hpp"

void fdn_component(const char* id, fdn_8_props_019* fdn_props_L, fdn_8_props_019* fdn_props_R) {
    PushID(id);
    if (ImGui::BeginTable("##fdn_table", 3)) {
        ImGui::TableNextColumn();
        if (ImGui::Button("get fdn patch")) {
            ImGui::SetClipboardText(fdn_props_L->to_str().c_str());
        }
        ImGui::NewLine();

        // one control for all fb gains
        if (SliderFloat("fb", &fdn_props_L->fb_gain, 0, 2) && fdn_props_R) {
            fdn_props_R->fb_gain = fdn_props_L->fb_gain;
        }

        // one control for all lpf filters
        if (SliderFloat(
                    "lpf_cut",
                    &fdn_props_L->lpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat) &&
            fdn_props_R) {
            fdn_props_R->lpf_cut = fdn_props_L->lpf_cut;
        }

        // one control for all hpf filters
        if (SliderFloat(
                    "hpf_cut",
                    &fdn_props_L->hpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat) &&
            fdn_props_R) {
            fdn_props_R->hpf_cut = fdn_props_L->hpf_cut;
        }

        ImGui::NewLine();

        ImGui::TableNextColumn();
        if (fdn_props_R) {
            for (auto [i, tap_L, tap_R] :
                 std::views::zip(std::views::iota(0, 8), fdn_props_L->taps, fdn_props_R->taps)) {
                PushID(&tap_L);

                if (SliderFloat("fdl_tap", &tap_L, 4, 12000)) {
                    tap_R = tap_L;
                }

                PopID();
            }
        } else {
            for (auto& tap_L : fdn_props_L->taps) {
                PushID(&tap_L);

                SliderFloat("fdl_tap", &tap_L, 4, 12000);

                PopID();
            }
        }

        ImGui::EndTable();
    }
    PopID();
}