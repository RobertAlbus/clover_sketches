#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "views/views.hpp"
using namespace ImGui;

#include "shared_props.hpp"

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
            if (ImGui::BeginTabItem("bass")) {
                view_bass("bass", props.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("cymbal")) {
                view_cymbal("cymbal", props.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("lead_a")) {
                view_lead_a("lead_a", props.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("lead_b")) {
                view_lead_b("lead_b", props.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("chord")) {
                view_chord("chord", props.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("pad")) {
                view_pad("pad", props.composition);
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
    params.appWindowParams.windowTitle = "Hello, Audio";
    params.fpsIdling.enableIdling      = true;
    HelloImGui::Run(params);
}