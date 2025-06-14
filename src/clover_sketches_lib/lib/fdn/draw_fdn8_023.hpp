#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "fdn8_023.hpp"
#include <ranges>

void draw_fdn8_023(const char* id, fdn8_023* fdn_L, fdn8_023* fdn_R = nullptr);

template <typename T>
concept draw_fdn8_023_concept = requires(T* fdn, float f, float q) {
    // props member and subfields
    { fdn->props } -> std::same_as<fdn8_props_023&>;
    { fdn->props.fb_gain } -> std::convertible_to<float&>;
    { fdn->props.lpf_cut } -> std::convertible_to<float&>;
    { fdn->props.hpf_cut } -> std::convertible_to<float&>;
    { fdn->props.taps } -> std::same_as<std::array<float, 8>&>;
    { fdn->props.to_str() } -> std::convertible_to<std::string>;

    // fdls member and m_max_idx access
    { fdn->fdls } -> std::same_as<std::array<clover::dsp::fdl_lagrange, 8>&>;
    { fdn->fdls[0].m_max_idx } -> std::convertible_to<float>;

    // fbs member
    { fdn->fbs } -> std::same_as<std::array<float, 8>&>;

    // fb_coeff member
    { fdn->fb_coeff } -> std::convertible_to<float&>;

    // filter setters
    { fdn->set_lpf(f) };
    { fdn->set_lpf(f, q) };
    { fdn->set_hpf(f) };
    { fdn->set_hpf(f, q) };
};

bool draw_tap_control(float& tap_L, float tap_min, float tap_max, float tap_range, float available_width);
bool draw_tap_control(
        float& tap_L, float& tap_R, float tap_min, float tap_max, float tap_range, float available_width);

template <draw_fdn8_023_concept fdn_t>
bool draw_fdn8_023_v2(const char* id, fdn_t& fdn) {
    ImGui::PushID(id);
    bool was_changed = false;

    if (ImGui::BeginTable("##fdn_table", 2)) {
        ImGui::TableNextColumn();
        if (ImGui::Button("get fdn patch")) {
            ImGui::SetClipboardText(fdn.props.to_str().c_str());
        }
        ImGui::NewLine();

        // one control for all fb gains
        was_changed |= SliderFloat("fb", &fdn.props.fb_gain, 0, 1.5);

        // one control for all lpf filters
        if (SliderFloat(
                    "lpf cut",
                    &fdn.props.lpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            fdn.set_lpf(fdn.props.lpf_cut);
            was_changed = true;
        }

        // one control for all hpf filters
        if (SliderFloat(
                    "hpf cut",
                    &fdn.props.hpf_cut,
                    20,
                    22000,
                    "%.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            fdn.set_hpf(fdn.props.hpf_cut);
            was_changed = true;
        }

        ImGui::NewLine();

        ImGui::TableNextColumn();
        float drag_speed = ImGui::GetIO().KeyShift ? 0.01f : 1.0f;

        float tap_max   = fdn.fdls[0].m_max_idx;
        float tap_min   = 3;
        float tap_range = tap_max - tap_min;

        float available_width = ImGui::GetContentRegionAvail().x;

        std::array<float, 8>& taps_L = fdn.props.taps;
        for (auto [i, tap_L] : std::views::zip(std::views::iota(0, 8), taps_L)) {
            float tap_slider_control = (tap_L - tap_min) / tap_range;
            was_changed |= draw_tap_control(
                    tap_L,  //
                    tap_min,
                    tap_max,
                    tap_range,
                    available_width);
        }

        ImGui::EndTable();    
    }
    ImGui::PopID();

    return was_changed;
}
