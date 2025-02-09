#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "composition/fdn/fdn.hpp"
#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
using namespace ImGui;

#include "composition.hpp"
#include "composition/kick.hpp"
#include "shared_props.hpp"

#include "gui/adsr.hpp"
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
                ImGui::TableNextColumn();
                // clang-format off
                std::string adsr_id_amp = "amp";
                ImGui::TextUnformatted(adsr_id_amp.c_str());
                adsr(adsr_id_amp, adsr_amp, kick.props.amp_a, kick.props.amp_d, kick.props.amp_s, kick.props.amp_r);
                ImGui::TableNextColumn();
                std::string adsr_id_cut = "cut";
                ImGui::TextUnformatted(adsr_id_cut.c_str());
                adsr(adsr_id_cut, adsr_cut, kick.props.cut_a, kick.props.cut_d, kick.props.cut_s, kick.props.cut_r);
                ImGui::TableNextColumn();
                std::string adsr_id_pitch = "pitch";
                ImGui::TextUnformatted(adsr_id_pitch.c_str());
                adsr(adsr_id_pitch, adsr_pitch, kick.props.pitch_a, kick.props.pitch_d, kick.props.pitch_s, kick.props.pitch_r);
                // clang-format on
                ImGui::EndTable();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("fdn")) {
                /*
                TODO
                - create GUI for FDN
                - use one set of controls to set both fdn_L and fdn_R

                */
                ImGui::BeginTable("##fdn_table", 2);
                ImGui::TableNextColumn();
                if (ImGui::Button("get fdn patch")) {
                    ImGui::SetClipboardText(fdn_L.props.to_str().c_str());
                }
                ImGui::SameLine();

                ImGui::TableNextColumn();

                ImGui::EndTable();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

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