// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"
#include "implot.h"

#include "context.hpp"

#include "controllers.hpp"

#include "gui/component/fdn_ui.hpp"
#include "gui/component/gui_logger_component.hpp"
#include "gui/component/kick_drum_gui.hpp"
#include "gui/component/mixer_ui.hpp"
#include "gui/component/peq.hpp"

void controller_mixer(const char* id, context& ctx);
void controller_kick(const char* id, context& ctx);
void controller_chord(const char* id, context& ctx);

std::vector<tabbed_controller> tabbed_controllers{
        // clang-format off
        {"mixer", controller_mixer},
        {"kick",  controller_kick},
        {"chord", controller_chord}
        // clang-format on
};

void controller_mixer(const char* id, context& ctx) {
    composition* comp = ctx.composition;

    ImGui::PushID(id);

    gui_logger_component(ctx);

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

void controller_kick(const char* id, context& ctx) {
    composition* comp = ctx.composition;

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

void controller_chord(const char* id, context& ctx) {
    composition* comp = ctx.composition;

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
