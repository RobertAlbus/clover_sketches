// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "draw_cymbal_038.hpp"

#include "clover/dsp/filter.hpp"
#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/_atom/drag_slider_025.hpp"
#include "lib/_molecule/adsr/draw_adsr_038.hpp"

void draw_cymbal_038(const char* id, cymbal_038& cymbal) {
    ImGui::PushID(id);
    ImGui::BeginGroup();  // bounding group

    ImGui::BeginGroup();
    if (ImGui::Button("get cymbal patch")) {
        ImGui::SetClipboardText(cymbal.props.to_str().c_str());
    }
    ImGui::Dummy({0, 10});
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::Text("amp");
    if (draw_adsr_038("##amp_adsr", {1000, 1000, 1, 30000}, cymbal.props.amp_adsr)) {
        cymbal.adsr_amp.set(
            cymbal.props.amp_adsr.a,
            cymbal.props.amp_adsr.d,
            cymbal.props.amp_adsr.s,
            cymbal.props.amp_adsr.r);
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Text("hpf cut");
    if (draw_adsr_038("##hpf_adsr", {1000, 1000, 1, 30000}, cymbal.props.cut_adsr)) {
        cymbal.adsr_cut.set(
            cymbal.props.cut_adsr.a,
            cymbal.props.cut_adsr.d,
            cymbal.props.cut_adsr.s,
            cymbal.props.cut_adsr.r);
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Dummy({100, 0});
    ImGui::SameLine();

    ImGui::BeginGroup();  // filter knobs
    ImGui::Dummy({0, 83});

    ImGui::BeginGroup();  // hpf knobs
    ImGui::PushID("bpf");
    ImGui::Text("hpf");
    ImGuiKnobs::Knob("f0", &cymbal.props.hpf_f0, 20, 24000);
    ImGui::SameLine();
    ImGuiKnobs::Knob("fmod", &cymbal.props.hpf_fmod_octaves, 0, 6);
    ImGui::SameLine();
    ImGuiKnobs::Knob("Q", &cymbal.props.hpf_Q, 0.1, 10);
    ImGui::PopID();
    ImGui::EndGroup();  // hpf knobs

    ImGui::SameLine();
    ImGui::Dummy({30, 0});
    ImGui::SameLine();

    ImGui::BeginGroup();  // bpf knobs
    ImGui::PushID("bpf");
    ImGui::Text("bpf");
    bool bpf_filter_change = false;
    bpf_filter_change |= ImGuiKnobs::Knob("f0", &cymbal.props.bpf_f0, 20, 24000);
    ImGui::SameLine();
    bpf_filter_change |= ImGuiKnobs::Knob("Q", &cymbal.props.bpf_Q, 0.1, 10);

    if (bpf_filter_change) {
        cymbal.band_pass.m_coeffs = clover::dsp::bpf(cymbal.fs, cymbal.props.bpf_f0, cymbal.props.bpf_Q);
    }
    ImGui::PopID();
    ImGui::EndGroup();  // bpf knobs
    ImGui::EndGroup();  // filter knobs

    ImGui::Dummy({0, 20});

    ImGui::BeginGroup();  // osc tuning
    for (auto& osc : cymbal.oscs) {
        float freq = osc.freq();
        ImGui::PushID(&osc);
        if (drag_slider_h_025(freq, 1, 24000)) {
            osc.freq(freq);
        }
        ImGui::PopID();
    }
    ImGui::EndGroup();  // osc tuning

    ImGui::Dummy({0, 20});

    ImGui::PopID();
    ImGui::EndGroup();  // bounding group
}
