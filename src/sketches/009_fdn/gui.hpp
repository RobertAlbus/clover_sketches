#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"

#include "composition.hpp"
#include "shared_props.hpp"

using namespace ImGui;

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts

    static float fb_coeff = props.composition->reverb_L.sections[0].fb_coefficient;

    // gui setup complete
    props.gui_ready.release();

    auto reverbs =
            std::views::zip(props.composition->reverb_L.sections, props.composition->reverb_R.sections);

    auto guiFunction = [&]() {
        ImGui::SliderFloat("verb_in", &props.composition->verb_in_gain, 0, 1);
        ImGui::SliderFloat("verb_mix", &props.composition->reverb_mix, 0, 1);
        if (ImGui::SliderFloat("fb time", &fb_coeff, 0.7, 1)) {
            for (auto [L, R] : reverbs) {
                L.fb_coefficient = fb_coeff;
                R.fb_coefficient = fb_coeff;
            }
        }

        for (auto [L, R] : reverbs) {
            if (ImGui::SliderFloat(
                        std::format("time##{}", static_cast<const void*>(&L)).c_str(), &L.fdl_tap, 4, 4790)) {
                R.fdl_tap = L.fdl_tap;
            }
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