#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "hello_imgui/hello_imgui.h"
#include "imgui.h"

#include "composition.hpp"
#include "composition/kick.hpp"
#include "shared_props.hpp"

using namespace ImGui;

void adsr(
        std::string id,
        float* adsr,
        settable& settable_a,
        settable& settable_d,
        settable& settable_s,
        settable& settable_r) {
    float width = 30;

    ImGui::PushID(id.c_str());
    bool set_a = VSliderFloat("##A_slider", ImVec2(width, 100), adsr + 0, 1, 1000, "");
    ImGui::SameLine();
    bool set_d = VSliderFloat("##D_slider", ImVec2(width, 100), adsr + 1, 1, 50000, "");
    ImGui::SameLine();
    bool set_s = VSliderFloat("##S_slider", ImVec2(width, 100), adsr + 2, 0, 1, "");
    ImGui::SameLine();
    bool set_r = VSliderFloat("##R_slider", ImVec2(width, 100), adsr + 3, 1, 1000, "");

    ImGui::PushItemWidth(width);
    set_a = set_a || ImGui::DragFloat("##A_spinner", adsr + 0, 2, 1, 1000, "%.0f");
    ImGui::SameLine();
    set_d = set_d || ImGui::DragFloat("##D_spinner", adsr + 1, 50, 1, 50000, "%.0f");
    ImGui::SameLine();
    set_s = set_s || ImGui::DragFloat("##S_spinner", adsr + 2, 0.1f, 0, 1, "%.2f");
    ImGui::SameLine();
    set_r = set_r || ImGui::DragFloat("##R_spinner", adsr + 3, 2, 1, 1000, "%.0f");
    ImGui::PopItemWidth();

    if (set_a) {
        settable_a.set(adsr[0]);
    }
    if (set_d) {
        settable_d.set(adsr[1]);
    }
    if (set_s) {
        settable_s.set(adsr[2]);
    }
    if (set_r) {
        settable_r.set(adsr[3]);
    }

    ImGui::PopID();
}

void GUI(shared_props& props) {
    props.audio_ready.acquire();
    // gui setup before audio starts

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
        if (ImGui::Button("get kick_drum patch")) {
            ImGui::SetClipboardText(kick.props.to_str().c_str());
        }

        if (VSliderFloat("Gain##amp", ImVec2(20, 100), &gain, 0, 1, "%.3f")) {
            kick.props.gain.set(gain);
        }

        // clang-format off
        ImGui::Separator();
        std::string adsr_id_amp = "amp";
        adsr(adsr_id_amp, adsr_amp, kick.props.amp_a, kick.props.amp_d, kick.props.amp_s, kick.props.amp_r);
        ImGui::Separator();
        std::string adsr_id_cut = "cut";
        adsr(adsr_id_cut, adsr_cut, kick.props.cut_a, kick.props.cut_d, kick.props.cut_s, kick.props.cut_r);
        ImGui::Separator();
        std::string adsr_id_pitch = "pitch";
        adsr(adsr_id_pitch, adsr_pitch, kick.props.pitch_a, kick.props.pitch_d, kick.props.pitch_s, kick.props.pitch_r);
        // clang-format on

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