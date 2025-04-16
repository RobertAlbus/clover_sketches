// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
using namespace ImGui;

#include "gui.hpp"
#include "shared_props.hpp"

#include "views/views.hpp"

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts
    props.gui_ready.release();

    auto guiFunction = [&]() {
        if (ImGui::BeginTabBar("Main Layout Tabs")) {
            if (ImGui::BeginTabItem("mixer")) {
                view_mixer("##kick_drum_gui", props.composition);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("kick")) {
                view_kick("kick", props.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("chord")) {
                view_chord("chord", props.composition);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::NewLine();
        ImGui::NewLine();
        if (ImGui::Button("Bye!")) {
            HelloImGui::GetRunnerParams()->appShallExit = true;
        }
        ImGui::Text("Framerate: %.2f", ImGui::GetIO().Framerate);
    };

    HelloImGui::RunnerParams params{};

    params.callbacks.EnqueueBeforeExit([&]() { props.gui_intent_to_exit.release(); });

    params.callbacks.ShowGui           = guiFunction;
    params.appWindowParams.windowTitle = "019_composition gui";
    params.fpsIdling.enableIdling      = true;
    HelloImGui::Run(params);
}