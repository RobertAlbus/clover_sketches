// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <string>

#include "imgui.h"
using namespace ImGui;

#include "draw_fdn8_000.hpp"

void draw_fdn8_000(const char* id, fdn8_000* fdn_L, fdn8_000* fdn_R) {
    PushID(id);
    if (ImGui::BeginTable("##fdn_table", 3)) {
        ImGui::TableNextColumn();
        if (ImGui::Button("get fdn patch")) {
            ImGui::SetClipboardText(fdn_L->props.to_str().c_str());
        }
        ImGui::NewLine();

        // one control for all fb gains
        if (SliderFloat("fb", &fdn_L->props.fb_gain, 0, 2) && fdn_R) {
            fdn_R->props.fb_gain = fdn_L->props.fb_gain;
        }

        // one control for all lpf filters
        if (SliderFloat(
                "lpf_cut",
                &fdn_L->props.lpf_cut,
                20,
                22000,
                "%.3f",
                ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat) &&
            fdn_R) {
            fdn_R->props.lpf_cut = fdn_L->props.lpf_cut;
        }

        // one control for all hpf filters
        if (SliderFloat(
                "hpf_cut",
                &fdn_L->props.hpf_cut,
                20,
                22000,
                "%.3f",
                ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat) &&
            fdn_R) {
            fdn_R->props.hpf_cut = fdn_L->props.hpf_cut;
        }

        ImGui::NewLine();

        ImGui::TableNextColumn();
        if (fdn_R) {
            for (auto [i, tap_L, tap_R] :
                 std::views::zip(std::views::iota(0, 8), fdn_L->props.taps, fdn_R->props.taps)) {
                PushID(&tap_L);

                if (SliderFloat("fdl_tap", &tap_L, 4, 12000)) {
                    tap_R = tap_L;
                }

                PopID();
            }
        } else {
            for (auto& tap_L : fdn_L->props.taps) {
                PushID(&tap_L);

                SliderFloat("fdl_tap", &tap_L, 4, 12000);

                PopID();
            }
        }

        ImGui::EndTable();
    }
    PopID();
}