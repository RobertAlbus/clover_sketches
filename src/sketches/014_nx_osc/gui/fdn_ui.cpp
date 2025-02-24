// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <atomic>
#include <ranges>
#include <string>

#include "imgui.h"
using namespace ImGui;

#include "composition/instruments/fdn.hpp"
#include "fdn_ui.hpp"

/*
std::array<settable, 8> taps{4, 4, 4, 4, 4, 4, 4, 4};
std::array<settable, 8> fb_gains{0, 0, 0, 0, 0, 0, 0, 0};
settable lpf_cut = 11621.206;
settable lpf_res = 0.707;
settable hpf_cut = 20;
settable hpf_res = 0.707;
*/

void fdn_component(const std::string& id, fdn_8_props_012& fdn_props_L, fdn_8_props_012& fdn_props_R) {
    PushID(id.c_str());
    ImGui::BeginTable("##fdn_table", 3);
    ImGui::TableNextColumn();
    if (ImGui::Button("get fdn patch")) {
        ImGui::SetClipboardText(fdn_props_L.to_str().c_str());
    }
    ImGui::NewLine();

    // one control for all fb gains
    if (SliderFloat("fb", &fdn_props_L.fb_gain.gui, 0, 2)) {
        fdn_props_L.fb_gain.set(fdn_props_L.fb_gain.gui);
        fdn_props_R.fb_gain.set(fdn_props_L.fb_gain.gui);
    }

    // one control for all lpf filters
    if (SliderFloat(
                "lpf_cut",
                &fdn_props_L.lpf_cut.gui,
                0,
                22000,
                "%.3f",
                ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
        fdn_props_L.lpf_cut.set(fdn_props_L.lpf_cut.gui);
        fdn_props_R.lpf_cut.set(fdn_props_L.lpf_cut.gui);
    }

    // one control for all lpf filters
    if (SliderFloat(
                "hpf_cut",
                &fdn_props_L.hpf_cut.gui,
                0,
                22000,
                "%.3f",
                ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
        fdn_props_L.hpf_cut.set(fdn_props_L.hpf_cut.gui);
        fdn_props_R.hpf_cut.set(fdn_props_L.hpf_cut.gui);
    }

    ImGui::NewLine();

    ImGui::BeginTable("##fdn_values_table", 4);
    ImGui::TableNextColumn();
    ImGui::TableNextColumn();
    Text("input");
    ImGui::TableNextColumn();
    Text("gui");
    ImGui::TableNextColumn();
    Text("output");
    ImGui::TableNextColumn();
    Text("fb gain");
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.fb_gain.gui);
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.fb_gain.gui);
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.fb_gain.output.load(std::memory_order_acquire));

    ImGui::TableNextColumn();
    Text("lpf cut");
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.lpf_cut.gui);
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.lpf_cut.gui);
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.lpf_cut.output.load(std::memory_order_acquire));

    ImGui::TableNextColumn();
    Text("hpf cut");
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.hpf_cut.gui);
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.hpf_cut.gui);
    ImGui::TableNextColumn();
    Text("%.2f", fdn_props_L.hpf_cut.output.load(std::memory_order_acquire));
    ImGui::EndTable();

    ImGui::TableNextColumn();
    for (auto [i, tap_L, tap_R] :
         std::views::zip(std::views::iota(0, 8), fdn_props_L.taps, fdn_props_R.taps)) {
        PushID(i);

        if (SliderFloat("fdl_tap", &tap_L.gui, 4, 12000)) {
            tap_L.set(tap_L.gui);
            tap_R.set(tap_L.gui);
        }

        PopID();
    }

    ImGui::EndTable();

    PopID();
}