#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <format>

#include "composition.hpp"
#include "gui/imgui_range_scalar.hpp"
#include "hello_imgui/hello_imgui.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "shared_props.hpp"

void GUI(shared_props& props) {
    props.audio_ready.acquire();

    // any setup needed  before audio begins

    bool resonator_0_active = true;
    float resonator_values[props.composition->num_resonators * 5];
    for (auto i : std::views::iota(0, props.composition->num_resonators)) {
        resonator_values[(i * 5) + 0] = props.composition->smoother[i].cutoff_lpf.get();
        resonator_values[(i * 5) + 1] = props.composition->smoother[i].cutoff_hpf.get();
        resonator_values[(i * 5) + 2] = props.composition->smoother[i].freq.get();
        resonator_values[(i * 5) + 3] = props.composition->smoother[i].gain_fb.get();
        resonator_values[(i * 5) + 4] = props.composition->smoother[i].gain_out.get();
    }

    props.gui_ready.release();

    auto guiFunction = [&]() {
        // static float range_min = 0.25f;
        // static float range_max = 0.75f;
        // ImGui::Text("Horizontal Range Slider:");
        // if (range_slider("##horiz_slider", &range_min, &range_max, 0.0f, 1.0f, ImVec2(200, 0))) {
        //     // range_slider returned true if value was changed
        //     // do something with the updated range_min and range_max
        // }

        for (auto i : std::views::iota(0, props.composition->num_resonators)) {
            if (RangeFloat(
                        std::format("##horiz_slider_{}", i + 1).c_str(),
                        &resonator_values[(i * 5) + 1],
                        &resonator_values[(i * 5) + 0],
                        10.f,
                        20000.f,
                        "%2.f",
                        ImGuiSliderFlags_None)) {
            }
            if (SliderFloat(
                        std::format("##horiz_slider_{}", i + 1).c_str(),
                        &resonator_values[(i * 5) + 2],
                        10.f,
                        20000.f,
                        "%2.f",
                        ImGuiSliderFlags_None)) {
                props.composition->smoother[i].freq.set(resonator_values[(i * 5) + 2]);
            }
        }

        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();

        static float verticals[6] = {25, 25, 25, 75, 75, 75};
        for (int i = 0; i < 3; ++i) {
            ImGui::SameLine();

            if (RangeFloat(
                        std::format("##vert_slider_{}", i + 1).c_str(),
                        &(verticals[i]),
                        &(verticals[3 + i]),
                        0.f,
                        100.f,
                        "%2.f",
                        ImGuiSliderFlags_None)) {
            }
        }

        ImGui::Separator();

        ImGui::SliderFloat(
                std::format("global decay").c_str(), &props.composition->global_decay_damper, 0.f, 1);

        if (ImGui::BeginTabBar("Resonator Tabs")) {
            for (auto i : std::views::iota(0, props.composition->num_resonators)) {
                if (ImGui::BeginTabItem(std::format("Resonator {}", i).c_str())) {
                    ImGui::BeginGroup();
                    ImGui::PushStyleVar(
                            ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));  // Adjust spacing (optional)

                    ImGui::DragFloatRange2(
                            std::format("{} cutoff", i).c_str(),
                            &resonator_values[(i * 5) + 0],
                            &resonator_values[(i * 5) + 1],
                            100.f,
                            10.f,
                            24000.f,
                            "%.2f",
                            nullptr,
                            ImGuiSliderFlags_Logarithmic);
                    ImGui::PopStyleVar();

                    ImGui::SliderFloat3(
                            std::format("{} cutoff", i).c_str(),
                            &resonator_values[(i * 5) + 0],
                            10.f,
                            24000.f,
                            "%.2f",
                            ImGuiSliderFlags_(ImGuiSliderFlags_Vertical) | ImGuiSliderFlags_Logarithmic);
                    // float& value_0 = resonator_values[(i * 5) + 0];
                    // if (ImGui::SliderFloat(
                    //             std::format("{} cutoff_lpf", i).c_str(),
                    //             &value_0,
                    //             10.f,
                    //             24000.f,
                    //             "%.2f",
                    //             ImGuiSliderFlags_Logarithmic)) {
                    //     props.composition->smoother[i].cutoff_lpf.set(value_0);
                    // }
                    // float& value_1 = resonator_values[(i * 5) + 1];
                    // if (ImGui::SliderFloat(
                    //             std::format("{} cutoff_hpf", i).c_str(),
                    //             &value_1,
                    //             10.f,
                    //             24000.f,
                    //             "%.2f",
                    //             ImGuiSliderFlags_Logarithmic)) {
                    //     props.composition->smoother[i].cutoff_hpf.set(value_1);
                    // }

                    // float& value_2 = resonator_values[(i * 5) + 2];
                    // if (ImGui::SliderFloat(
                    //             std::format("{} freq", i).c_str(),
                    //             &value_2,
                    //             10.f,
                    //             24000.f,
                    //             "%.2f",
                    //             ImGuiSliderFlags_Logarithmic)) {
                    //     props.composition->smoother[i].freq.set(value_2);
                    // }

                    // float& value_3 = resonator_values[(i * 5) + 3];
                    // if (ImGui::SliderFloat(
                    //             std::format("{} gain_fb", i).c_str(),
                    //             &value_3,
                    //             0.9f,
                    //             1.1f,
                    //             "%.5f",
                    //             ImGuiSliderFlags_Logarithmic)) {
                    //     props.composition->smoother[i].gain_fb.set(value_3);
                    // }
                    // float& value_4 = resonator_values[(i * 5) + 4];
                    // if (ImGui::SliderFloat(
                    //             std::format("{} gain_out", i).c_str(),
                    //             &value_4,
                    //             0.f,
                    //             1.f,
                    //             "%.2f",
                    //             ImGuiSliderFlags_Logarithmic)) {
                    //     props.composition->smoother[i].gain_out.set(value_4);
                    // }

                    ImGui::EndGroup();
                    ImGui::EndTabItem();
                }
            }
        }
        ImGui::EndTabBar();

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