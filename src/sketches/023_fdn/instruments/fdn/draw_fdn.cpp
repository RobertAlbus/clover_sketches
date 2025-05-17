// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>
#include <string>

#include "imgui.h"
using namespace ImGui;

#include "draw_fdn.hpp"

void draw_fdn8_023(const char* id, fdn8_023* fdn_L, fdn8_023* fdn_R) {
    PushID(id);
    if (ImGui::BeginTable("##fdn_table", 2)) {
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
                    "lpf cut",
                    &fdn_L->props.lpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            fdn_L->set_lpf(fdn_L->props.lpf_cut);
            if (fdn_R) {
                fdn_R->set_lpf(fdn_L->props.lpf_cut);
            }
        }

        // one control for all hpf filters
        if (SliderFloat(
                    "hpf cut",
                    &fdn_L->props.hpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            fdn_L->set_hpf(fdn_L->props.hpf_cut);
            if (fdn_R) {
                fdn_R->set_hpf(fdn_L->props.hpf_cut);
            }
        }

        ImGui::NewLine();

        ImGui::TableNextColumn();
        float drag_speed = ImGui::GetIO().KeyShift ? 0.01f : 1.0f;

        float tap_max   = fdn_L->fdls[0].m_max_idx;
        float tap_min   = 3;
        float tap_range = tap_max - tap_min;

        float available_width = ImGui::GetContentRegionAvail().x;

        if (fdn_R) {
            for (auto [i, tap_L, tap_R] :
                 std::views::zip(std::views::iota(0, 8), fdn_L->props.taps, fdn_R->props.taps)) {
                PushID(&tap_L);

                float tap_value = (tap_L - tap_min) / tap_range;
                PushItemWidth(available_width * 0.1f);
                bool changed_drag = DragFloat(
                        "##tap_drag",
                        &tap_L,
                        0.01,
                        tap_min,
                        tap_max,
                        "%.3f",
                        ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
                PopItemWidth();
                SameLine();
                PushItemWidth(available_width * 0.9f);
                bool changed_slider = SliderFloat(
                        "##tap_slider",
                        &tap_value,
                        0,
                        1,
                        "",
                        ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
                PopItemWidth();

                if (changed_slider) {
                    tap_L = (tap_value * tap_range) + tap_min;
                    tap_R = tap_L;
                }
                PopID();
            }
        } else {
            for (auto& tap_L : fdn_L->props.taps) {
                PushID(&tap_L);

                float tap_value = (tap_L - tap_min) / tap_range;
                PushItemWidth(available_width * 0.1f);
                bool changed_drag = DragFloat(
                        "##tap_drag",
                        &tap_L,
                        0.01,
                        tap_min,
                        tap_max,
                        "%.3f",
                        ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
                PopItemWidth();
                SameLine();
                PushItemWidth(available_width * 0.9f);
                bool changed_slider = SliderFloat(
                        "##tap_slider",
                        &tap_value,
                        0,
                        1,
                        "",
                        ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_Logarithmic);
                PopItemWidth();

                if (changed_slider) {
                    tap_L = (tap_value * tap_range) + tap_min;
                }
                PopID();
            }
        }

        ImGui::EndTable();
    }
    PopID();
}
