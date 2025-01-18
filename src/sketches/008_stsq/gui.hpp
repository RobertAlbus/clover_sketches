#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include "composition.hpp"
#include "hello_imgui/hello_imgui.h"
#include "shared_props.hpp"

using namespace ImGui;

void GUI(shared_props& props) {
    props.audio_ready.acquire();

    // any setup needed  before audio begins
    auto& hh = props.composition->hh;

    props.gui_ready.release();

    auto guiFunction = [&]() {};

    HelloImGui::RunnerParams params{};

    params.callbacks.EnqueueBeforeExit([&]() { props.gui_intent_to_exit.release(); });

    params.callbacks.ShowGui           = guiFunction;
    params.appWindowParams.windowTitle = "Hello, Audio";
    params.fpsIdling.enableIdling      = true;
    HelloImGui::Run(params);
}