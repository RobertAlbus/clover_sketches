#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "hello_imgui/hello_imgui.h"

#include "implot_example.h"
#include "shared_props.hpp"
#include <numbers>

void GUI(shared_props& props) {
    props.audio_ready.acquire();

    // any setup needed  before audio begins

    float freq  = props.composition->osc.freq();
    float phase = props.composition->osc.phase();

    ImPlot::CreateContext();

    props.gui_ready.release();

    auto guiFunction = [&]() {
        ImGui::PushItemWidth(400);

        ImGui::SliderFloat("gain", &props.composition->gain, 0.f, 1.f);
        if (ImGui::SliderFloat("freq", &freq, 10.f, 24000.f, "%.2f", ImGuiSliderFlags_Logarithmic)) {
            props.composition->osc.freq(freq);
        }

        if (ImGui::SliderFloat("phase", &phase, 0.f, std::numbers::pi_v<float>)) {
            props.composition->osc.phase(phase);
        }

        ImGui::PopItemWidth();

        demo_realtime_implot();

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