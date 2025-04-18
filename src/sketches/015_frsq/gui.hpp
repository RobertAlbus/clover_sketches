#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>
#include <ranges>

#include "composition/instruments/fdn.hpp"
#include "gui/audio_meter_poc.hpp"
#include "gui/kick_drum_gui.hpp"
#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
using namespace ImGui;

#include "composition.hpp"
#include "composition/instruments/kick.hpp"
#include "shared_props.hpp"

#include "gui/fdn_ui.hpp"
#include "gui/mixer.hpp"

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts

    fdn_8_012& fdn_L = props.composition->kick_fdn_L;
    fdn_8_012& fdn_R = props.composition->kick_fdn_R;

    kick_drum& kick          = props.composition->kick;
    static float gain        = kick.props.trim.output;
    static float adsr_amp[4] = {
            kick.props.amp_a.output,
            kick.props.amp_d.output,
            kick.props.amp_s.output,
            kick.props.amp_r.output,
    };
    static float adsr_cut[4] = {
            kick.props.cut_a.output,
            kick.props.cut_d.output,
            kick.props.cut_s.output,
            kick.props.cut_r.output,
    };
    static float adsr_pitch[4] = {
            kick.props.pitch_a.output,
            kick.props.pitch_d.output,
            kick.props.pitch_s.output,
            kick.props.pitch_r.output,
    };

    fdn_L.props.fb_gain.gui = fdn_L.props.fb_gain.load_output();

    for (auto [L, R, P] : std::views::zip(fdn_L.props.taps, fdn_R.props.taps, fdn_patch.taps)) {
        L.gui = L.load_output();
        R.gui = L.gui;
    }

    // gui setup complete
    props.gui_ready.release();

    auto guiFunction = [&]() {
        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("mix")) {
                // Content for Tab 2
                mixer(&(props.composition->kick_mix),
                      &(props.composition->verb_in_gain),
                      &(props.composition->reverb_mix));

                static float meter_value = 0;
                audio_meter("meter", meter_value, ImVec2(30, 100));
                ImGui::VSliderFloat("##meter_control", ImVec2(30, 100), &meter_value, 0, 2);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("kick", nullptr)) {
                kick_drum_gui("##kick_drum_gui", kick.props);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("fdn")) {
                std::string fdn_component_id = {"fdn_ui"};
                fdn_component(fdn_component_id, fdn_L.props, fdn_R.props);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

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