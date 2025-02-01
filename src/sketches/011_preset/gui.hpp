#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"

#include "composition.hpp"
#include "shared_props.hpp"

using namespace ImGui;

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts

    // gui setup complete
    props.gui_ready.release();

    auto guiFunction = [&]() {
        ImGui::Separator();

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