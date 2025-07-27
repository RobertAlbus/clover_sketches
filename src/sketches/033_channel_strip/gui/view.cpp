// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/logging/draw_gui_log_canvas.hpp"
#include "lib/transport/transport_ui_028.hpp"

#include "controller/controllers.hpp"
#include "graph/graph.hpp"
#include "view.hpp"

view::view(std::string project_name, sequencers& sqs, signal_graph& graph, log_bus_000& logger)
    : project_name{std::move(project_name)},
      sqs{sqs},
      graph{graph},
      logger{logger},
      transport([&](float bar) { sqs.play_from_bar(bar); }, [&]() { sqs.play(); }, [&]() { sqs.stop(); }) {
    tabs = std::move(create_tabs());
}

std::vector<std::unique_ptr<tabbed_controller>> view::create_tabs() {
    std::vector<std::unique_ptr<tabbed_controller>> new_tabs;
    new_tabs.emplace_back(std::make_unique<controller_mixer>("mixer", sqs, graph));
    new_tabs.emplace_back(std::make_unique<controller_kick>("kick", sqs, graph));
    new_tabs.emplace_back(std::make_unique<controller_snare>("snare", sqs, graph));
    new_tabs.emplace_back(std::make_unique<controller_ride>("ride", sqs, graph));
    new_tabs.emplace_back(std::make_unique<controller_chord>("chord", sqs, graph));
    return new_tabs;
}

bool view::draw() {
    // Fullscreen Docking Node (single node replaces window content)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("main_area", nullptr, window_flags);

    ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // ----------------------------------------------------------------
    // KEY HANDLING

    if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_Space)) {
        transport.toggle_state_play();
    } else if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
        transport.toggle_state_start();
    } else if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_L)) {
        show_log_canvas = !show_log_canvas;
    }

    // ----------------------------------------------------------------
    // MENU BAR

    transport.draw();

    ImGui::SameLine();
    ImGui::Checkbox("logs", &show_log_canvas);

    ImVec2 title_size = ImGui::CalcTextSize(project_name.c_str());
    ImGui::SameLine((ImGui::GetWindowWidth() / 2) - (title_size.x / 2));
    ImGui::Text("%s", project_name.c_str());

    ImGui::SameLine(ImGui::GetWindowWidth() - 200);
    ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
    ImGui::SameLine();
    if (ImGui::Button(use_dark_theme ? "light" : "dark")) {
        use_dark_theme = !use_dark_theme;
        if (use_dark_theme) {
            ImGui::StyleColorsDark();
        } else {
            ImGui::StyleColorsLight();
        }
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 40);
    bool should_continue = true;
    if (ImGui::Button(" x ")) {
        should_continue = false;
    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    if (show_log_canvas) {
        draw_gui_log_canvas_000("log_canvas", canvas, logger, nullptr);
    }

    if (ImGui::BeginTabBar("main layout tabs")) {
        for (auto& tabbed_controller : tabs) {
            if (ImGui::BeginTabItem(tabbed_controller->name)) {
                float constrained_width = ImGui::GetWindowWidth() - (2 * ImGui::GetStyle().WindowPadding.x);
                float constrained_height =
                    ImGui::GetWindowHeight() - ImGui::GetCursorPosY() - (ImGui::GetStyle().WindowPadding.y);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(40.0f, 20.0f));
                ImGui::BeginChild(
                    "tab area",
                    ImVec2(constrained_width, constrained_height),
                    0,
                    ImGuiWindowFlags_AlwaysUseWindowPadding);
                tabbed_controller->draw();

                ImGui::PopStyleVar();
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
    // ----------------------------------------------------------------

    ImGui::End();  // end "main_area"
    return should_continue;
}
