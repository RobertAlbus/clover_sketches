#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "views/views.hpp"
using namespace ImGui;

#include "shared_props.hpp"

void GUI(context& ctx) {
    ctx.audio_ready.acquire();
    // gui setup before audio starts
    ctx.gui_ready.release();

    auto guiFunction = [&]() {
        if (ImGui::BeginTabBar("Main Layout Tabs")) {
            if (ImGui::BeginTabItem("mixer")) {
                controller_mixer("##kick_drum_gui", ctx.composition);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("kick")) {
                controller_kick("kick", ctx.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("bass")) {
                view_bass("bass", ctx.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("cymbal")) {
                view_cymbal("cymbal", ctx.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("lead_a")) {
                view_lead_a("lead_a", ctx.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("lead_b")) {
                view_lead_b("lead_b", ctx.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("chord")) {
                controller_chord("chord", ctx.composition);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("pad")) {
                view_pad("pad", ctx.composition);
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

    params.callbacks.EnqueueBeforeExit([&]() { ctx.gui_intent_to_exit.release(); });

    params.callbacks.ShowGui           = guiFunction;
    params.appWindowParams.windowTitle = "019_composition gui";
    params.fpsIdling.enableIdling      = true;
    HelloImGui::Run(params);
}