#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "instruments/fdn.hpp"
#include <print>
using namespace ImGui;

#include "shared_props.hpp"

#include "visual_components/fdn_ui.hpp"

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts
    props.gui_ready.release();

    auto guiFunction = [&]() {
        if (ImGui::Button("get mixer patch")) {
            ImGui::SetClipboardText(props.composition->mix.to_str().c_str());
        }
        ImGui::NewLine();
        fdn_component(
                "hh_reverb",
                props.composition->cymbals.hh_verb_L.props,
                props.composition->cymbals.hh_verb_R.props);
        ImGui::NewLine();
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