// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/logging/logger.hpp"

#include "controller/controllers.hpp"
#include "graph/graph.hpp"
#include "view.hpp"

view::view(signal_graph& graph, log_bus_000& logger) : graph{graph}, logger{logger} {
    tabs = std::move(create_tabs());
}

std::vector<tabbed_controller> view::create_tabs() {
    return {
            // clang-format off
            {"mixer", controller_mixer},
            {"kick",  controller_kick},
            {"ride",  controller_ride},
            {"chord",  controller_chord},
            // clang-format on
    };
}

bool view::draw() {
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

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // ----------------------------------------------------------------
    // TABS

    if (ImGui::BeginTabBar("Main Layout Tabs")) {
        for (auto& tabbed_controller : tabs) {
            if (ImGui::BeginTabItem(tabbed_controller.name)) {
                tabbed_controller.controller(tabbed_controller.name, graph, logger);

                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }

    // ----------------------------------------------------------------

    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::Text("Framerate: %.2f", ImGui::GetIO().Framerate);

    if (ImGui::Button("Bye!")) {
        ImGui::End();
        bool should_continue = false;
        return should_continue;
    }

    ImGui::End();  // End "MainDockSpaceHost"
    bool should_continue = true;
    return should_continue;
}
