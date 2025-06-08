// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/logging/logger.hpp"

#include "controller/controllers.hpp"
#include "graph/graph.hpp"
#include "view.hpp"

view::view(sequencers& sqs, signal_graph& graph, log_bus_000& logger)
    : sqs{sqs}, graph{graph}, logger{logger} {
    tabs = std::move(create_tabs());
}

std::vector<std::unique_ptr<tabbed_controller>> view::create_tabs() {
    std::vector<std::unique_ptr<tabbed_controller>> new_tabs;
    new_tabs.emplace_back(std::make_unique<controller_mixer>("mixer", sqs, graph, logger));
    new_tabs.emplace_back(std::make_unique<controller_kick>("kick", sqs, graph, logger));
    new_tabs.emplace_back(std::make_unique<controller_ride>("ride", sqs, graph, logger));
    new_tabs.emplace_back(std::make_unique<controller_chord>("chord", sqs, graph, logger));
    return new_tabs;
}

bool view::draw() {
    // Fullscreen Docking Node (single node replaces window content)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_MenuBar;

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
            if (ImGui::BeginTabItem(tabbed_controller->name)) {
                tabbed_controller->draw();

                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }

    // ----------------------------------------------------------------

    ImGui::NewLine();
    ImGui::NewLine();

    ImGui::Text("Framerate: %.2f", ImGui::GetIO().Framerate);

    bool should_continue = !ImGui::Button("Bye!");

    ImGui::End();  // End "MainDockSpaceHost"
    return should_continue;
}
