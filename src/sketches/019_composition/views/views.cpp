// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "views.hpp"
#include "imgui.h"
#include "visual_components/fdn_ui.hpp"
#include "visual_components/kick_drum_gui.hpp"
#include "visual_components/mixer_ui.hpp"
#include "visual_components/peq.hpp"

void view_mixer(const char* id, composition* comp) {
    mixer_component("mix", &comp->mix);
}

void view_kick(const char* id, composition* comp) {
    kick_drum_gui("kick_synth", comp->kick.drum);
    if (ImGui::BeginTable("##kick_table", 2)) {
        ImGui::TableNextColumn();
        peq_gui("##kick_preverb_peq", comp->kick.preverb_peq);
        ImGui::TableNextColumn();
        peq_gui("##kick_out_peq", comp->kick.out_peq);
        ImGui::EndTable();
    }
    fdn_component("##kick_fdn", &comp->kick.verb.props, nullptr);
}

void view_bass(const char* id, composition* comp) {
    peq_gui("##bass_out_peq", comp->bass.out_peq);
}

void view_cymbal(const char* id, composition* comp) {
    peq_gui("##hh reverb", comp->cymbals.ride_peq);
    fdn_component(
            "hh_reverb",
            &comp->cymbals.hh_verb_L.props,
            &comp->cymbals.hh_verb_R.props);
    ImGui::Separator();

    // TODO: need a cymbal GUI.
    //       - cymbals sound alright for now though
    //       - not a high priority at this moment

    if (ImGui::BeginTable("##cymbal_peq_table", 3)) {

        ImGui::TableNextColumn();
        ImGui::Text("hh preverb peq");
        peq_gui("##hh_preverb_peq", comp->cymbals.hh_preverb_peq);
        
        ImGui::TableNextColumn();
        ImGui::Text("hh out peq");
        peq_gui("##hh_peq", comp->cymbals.hh_peq);
        
        ImGui::TableNextColumn();
        ImGui::Text("ride peq");
        peq_gui("##hh_peq", comp->cymbals.ride_peq);
        ImGui::EndTable();
    }
}

void view_lead_a(const char* id, composition* comp) {
}

void view_lead_b(const char* id, composition* comp) {
}

void view_chord(const char* id, composition* comp) {
}

void view_pad(const char* id, composition* comp) {
}
