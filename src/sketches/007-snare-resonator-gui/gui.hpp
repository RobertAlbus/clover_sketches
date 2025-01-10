#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <format>

#include "hello_imgui/hello_imgui.h"

#include "shared_props.hpp"

void GUI(shared_props& props) {
    props.audio_ready.acquire();

    // any setup needed  before audio begins

    props.gui_ready.release();

    auto guiFunction = [&]() {
        ImGui::PushItemWidth(400);

        ImGui::SliderFloat(
                std::format("global decay").c_str(), &props.composition->global_decay_damper, 0.f, 1);

        for (auto i : std::views::iota(0, props.composition->num_resonators)) {
            ImGui::SliderFloat(
                    std::format("fdl size {}", i).c_str(),
                    &props.composition->resonator_idx[i],
                    4.f,
                    float(props.composition->fdl_length));

            ImGui::SliderFloat(
                    std::format("fdl fdbk {}", i).c_str(),
                    &props.composition->resonator_decay_coeffs[i],
                    0.f,
                    1.f);

            ImGui::SliderFloat(
                    std::format("fdl out {}", i).c_str(), &props.composition->resonator_gains[i], 0.f, 1.f);
            ImGui::Separator();
        }

        ImGui::PopItemWidth();

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