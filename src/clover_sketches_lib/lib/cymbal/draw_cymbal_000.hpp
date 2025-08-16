#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/filter.hpp"
#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/_atom/drag_slider_025.hpp"
#include "lib/_molecule/adsr/draw_adsr.hpp"
#include "lib/cymbal/cymbal_000.hpp"

void draw_cymbal_000(const char* id, cymbal_000& cymbal) {
    ImGui::PushID(id);

    if (ImGui::Button("get cymbal patch")) {
        ImGui::SetClipboardText(cymbal.props.to_str().c_str());
    }
    if (ImGui::BeginTable(id, 3)) {
        ImGui::TableSetupColumn("amp");
        // ImGui::TableNextColumn();
        ImGui::TableSetupColumn("hpf");
        // ImGui::TableNextColumn();
        ImGui::TableSetupColumn("filters");
        ImGui::TableHeadersRow();

        ImGui::TableNextColumn();
        if (draw_adsr_000(
                "##amp_adsr",
                {1000, 1000, 1, 30000},
                cymbal.props.amp_a,
                cymbal.props.amp_d,
                cymbal.props.amp_s,
                cymbal.props.amp_r)) {
            cymbal.adsr_amp.set(
                cymbal.props.amp_a, cymbal.props.amp_d, cymbal.props.amp_s, cymbal.props.amp_r);
        }

        ImGui::TableNextColumn();

        if (draw_adsr_000(
                "##hpf_adsr",
                {1000, 1000, 1, 30000},
                cymbal.props.cut_a,
                cymbal.props.cut_d,
                cymbal.props.cut_s,
                cymbal.props.cut_r)) {
            cymbal.adsr_cut.set(
                cymbal.props.cut_a, cymbal.props.cut_d, cymbal.props.cut_s, cymbal.props.cut_r);
        }

        ImGui::TableNextColumn();
        ImGuiKnobs::Knob("hpf f0", &cymbal.props.hpf_f0, 20, 24000);
        ImGui::SameLine();
        ImGuiKnobs::Knob("hpf fmod", &cymbal.props.hpf_fmod_octaves, 0, 6);
        ImGui::SameLine();
        ImGuiKnobs::Knob("hpf Q", &cymbal.props.hpf_Q, 0.1, 10);

        bool filter_change = false;
        filter_change |= ImGuiKnobs::Knob("bpf f0", &cymbal.props.bpf_f0, 20, 24000);
        ImGui::SameLine();
        filter_change |= ImGuiKnobs::Knob("bpf Q", &cymbal.props.bpf_Q, 0.1, 10);

        if (filter_change) {
            cymbal.band_pass.m_coeffs = clover::dsp::bpf(cymbal.fs, cymbal.props.bpf_f0, cymbal.props.bpf_Q);
        }

        ImGui::EndTable();
    }
    for (auto& osc : cymbal.oscs) {
        float freq = osc.freq();
        ImGui::PushID(&osc);
        if (drag_slider_h_025(freq, 1, 24000)) {
            osc.freq(freq);
        }
        ImGui::PopID();
    }
    ImGui::PopID();
}
