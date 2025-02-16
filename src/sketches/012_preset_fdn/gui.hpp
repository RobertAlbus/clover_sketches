#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>
#include <ranges>

#include "composition/fdn/fdn.hpp"
#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
using namespace ImGui;

#include "composition.hpp"
#include "composition/kick.hpp"
#include "shared_props.hpp"

#include "gui/adsr.hpp"
#include "gui/fdn_ui.hpp"
#include "gui/mixer.hpp"

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts

    fdn_8_012& fdn_L = props.composition->fdn_L;
    fdn_8_012& fdn_R = props.composition->fdn_R;

    kick_drum& kick          = props.composition->kick;
    static float gain        = kick.props.gain.output;
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

    fdn_L.props.fb_gain.gui = fdn_L.props.fb_gain.output.load(std::memory_order_acquire);

    for (auto [L, R, P] : std::views::zip(fdn_L.props.taps, fdn_R.props.taps, fdn_patch.taps)) {
        L.gui = L.output.load(std::memory_order_acquire);
        R.gui = L.gui;
    }

    // gui setup complete
    props.gui_ready.release();

    auto guiFunction = [&]() {
        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("mix")) {
                // Content for Tab 2
                mixer(&(props.composition->loop_mix),
                      &(props.composition->verb_in_gain),
                      &(props.composition->reverb_mix));

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("kick")) {
                ImGui::BeginTable("##kick_drum_table", 4);
                ImGui::TableNextColumn();
                if (ImGui::Button("get kick_drum patch")) {
                    ImGui::SetClipboardText(kick.props.to_str().c_str());
                }
                ImGui::SameLine();
                if (VSliderFloat("Gain##amp", ImVec2(20, 100), &gain, 0, 1, "%.3f")) {
                    kick.props.gain.set(gain);
                }

                adsr_ranges ranges = {1000, 40000, 1, 1000};
                // clang-format off
                ImGui::TableNextColumn();
                ImGui::TextUnformatted("amplitude");
                adsr("kick_amp", ranges, kick.props.amp_a, kick.props.amp_d, kick.props.amp_s, kick.props.amp_r);

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("kick_cut");
                ranges.d_max = 20000;
                adsr("kick_cut", ranges, kick.props.cut_a, kick.props.cut_d, kick.props.cut_s, kick.props.cut_r);

                ImGui::TableNextColumn();
                ImGui::TextUnformatted("kick_pitch");
                ranges.d_max = 10000;
                adsr("kick_pitch", ranges, kick.props.pitch_a, kick.props.pitch_d, kick.props.pitch_s, kick.props.pitch_r);
                // clang-format on

                ImGui::EndTable();

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