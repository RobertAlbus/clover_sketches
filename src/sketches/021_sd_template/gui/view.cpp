// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "context.hpp"

#include "controller/controllers.hpp"
#include "view.hpp"

void view_setup(context& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts
    props.gui_ready.release();
}

bool view_draw(context& props) {
    // Fullscreen Docking Node (single node replaces window content)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("MainDockSpaceHost", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    // Optional: create a dockspace, even if unused
    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // ----------------

    if (ImGui::BeginTabBar("Main Layout Tabs")) {
        if (ImGui::BeginTabItem("mixer")) {
            controller_mixer("##kick_drum_gui", props.composition);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("kick")) {
            controller_kick("kick", props.composition);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("chord")) {
            controller_chord("chord", props.composition);
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::NewLine();
    ImGui::NewLine();
    if (ImGui::Button("Bye!")) {
        ImGui::End();
        return false;
    }
    ImGui::Text("Framerate: %.2f", ImGui::GetIO().Framerate);

    ImGui::End();  // End "MainDockSpaceHost"

    return true;
}
