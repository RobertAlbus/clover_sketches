#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <format>
#include <ranges>

#include "composition.hpp"
#include "hello_imgui/hello_imgui.h"

#include "imgui.h"
#include "shared_props.hpp"

using namespace ImGui;

void GUI(shared_props& props) {
    props.audio_ready.acquire();

    // any setup needed  before audio begins
    auto& hh = props.composition->hh;

    float osc_freq[hh.num_oscs];
    for (auto i : std::views::iota(0, hh.num_oscs))
        osc_freq[i] = hh.osc_freq[i];

    float adsr_vals[8] = {hh.amp_a, hh.amp_d, hh.amp_s, hh.amp_r, hh.cut_a, hh.cut_d, hh.cut_s, hh.cut_r};

    props.gui_ready.release();

    auto guiFunction = [&]() {
        NewLine();
        for (auto i : std::views::iota(0, hh.num_oscs)) {
            // SameLine();
            if (SliderFloat(
                        std::format("##freq_{}", i + 1).c_str(),
                        &osc_freq[i],
                        10.f,
                        20000.f,
                        "%2.2f",
                        ImGuiSliderFlags_Logarithmic)) {
                hh.osc[i].freq(osc_freq[i]);
            }
        }

        NewLine();

        if (SliderFloat(
                    "##amp_a",
                    &adsr_vals[0],
                    1.f,
                    20000.f,
                    "%2.2f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_amp.attack(adsr_vals[0]);
        }
        if (SliderFloat(
                    "##amp_d",
                    &adsr_vals[1],
                    0.f,
                    20000.f,
                    "%2.2f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_amp.decay(adsr_vals[1]);
        }
        if (SliderFloat(
                    "##amp_s",
                    &adsr_vals[2],
                    0.f,
                    1.f,
                    "%2.2f",
                    ImGuiSliderFlags_None | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_amp.sustain(adsr_vals[2]);
        }
        if (SliderFloat(
                    "##amp_r",
                    &adsr_vals[3],
                    0.f,
                    20000.f,
                    "%2.3f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_amp.release(adsr_vals[3]);
        }

        NewLine();

        if (SliderFloat(
                    "##cut_a",
                    &adsr_vals[4],
                    0.f,
                    20000.f,
                    "%2.2f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_cut.attack(adsr_vals[4]);
        }
        if (SliderFloat(
                    "##cut_d",
                    &adsr_vals[5],
                    0.f,
                    20000.f,
                    "%2.2f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_cut.decay(adsr_vals[5]);
        }
        if (SliderFloat(
                    "##cut_s",
                    &adsr_vals[6],
                    0.f,
                    1.f,
                    "%2.3f",
                    ImGuiSliderFlags_None | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_cut.sustain(adsr_vals[6]);
        }
        if (SliderFloat(
                    "##cut_r",
                    &adsr_vals[7],
                    0.f,
                    20000.f,
                    "%2.2f",
                    ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat)) {
            hh.adsr_cut.release(adsr_vals[7]);
        }

        if (ImGui::Button("Bye!")) {
            HelloImGui::GetRunnerParams()->appShallExit = true;
        }
        ImGui::Text("Framerate: %.2f", ImGui::GetIO().Framerate);
    };

    HelloImGui::RunnerParams params{};

    params.callbacks.EnqueueBeforeExit([&]() { props.gui_intent_to_exit.release(); });

    params.callbacks.ShowGui           = guiFunction;
    params.appWindowParams.windowTitle = "Hello, Audio";
    params.fpsIdling.enableIdling      = true;
    HelloImGui::Run(params);
}