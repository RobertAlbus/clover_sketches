// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "implot.h"

#include "views.hpp"

#include "visual_components/bar_logger_component.hpp"
#include "visual_components/fdn_ui.hpp"
#include "visual_components/kick_drum_gui.hpp"
#include "visual_components/mixer_ui.hpp"
#include "visual_components/peq.hpp"

void view_mixer(const char* id, composition* comp) {
    ImGui::PushID(id);

    bar_logger_component(*comp);

    static double x = 0.5f;
    static double y = 0.5f;

    static bool show_plot = false;
    ImGui::Checkbox("show plot", &show_plot);
    if (show_plot) {
        if (ImPlot::BeginPlot("Draggable Point")) {
            ImPlot::DragPoint(0, &x, &y, ImVec4(1, 0, 0, 1), 4, ImPlotDragToolFlags_None);
            ImPlot::EndPlot();
        }
    }

    mixer_component("mix", &comp->mix);
    peq_gui("##master_peq", comp->main_bus.eq);

    ImGui::PopID();
}

void view_kick(const char* id, composition* comp) {
    ImGui::PushID(id);

    kick_drum_gui("kick_synth", comp->kick.drum);
    if (ImGui::BeginTable("##kick_table", 2)) {
        ImGui::TableNextColumn();
        peq_gui("##kick_preverb_peq", comp->kick.preverb_peq);
        ImGui::TableNextColumn();
        ImGui::Text("kick postverb peq");
        peq_gui("##kick_out_peq", comp->kick.out_peq);
        ImGui::EndTable();
    }
    fdn_component("##kick_fdn", &comp->kick.verb, nullptr);

    ImGui::PopID();
}

void view_chord(const char* id, composition* comp) {
    ImGui::PushID(id);
    fdn_component("fdn", &comp->synth.chord_verb_L, &comp->synth.chord_verb_R);
    if (ImGui::BeginTable("##peq_table", 2)) {
        ImGui::TableNextColumn();
        ImGui::Text("chord preverb peq");
        peq_gui("##preverb_peq", comp->synth.chord_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("chord out peq");
        peq_gui("##postverb_peq", comp->synth.chord_peq);

        ImGui::EndTable();
    }
    ImGui::PopID();
}
