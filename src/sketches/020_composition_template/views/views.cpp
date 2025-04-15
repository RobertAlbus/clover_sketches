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
    ImGui::PushID(id);

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

void view_bass(const char* id, composition* comp) {
    ImGui::PushID(id);

    peq_gui("##bass_out_peq", comp->bass.out_peq);

    ImGui::PopID();
}

void view_cymbal(const char* id, composition* comp) {
    ImGui::PushID(id);

    fdn_component("hh_reverb", &comp->cymbals.hh_verb_L, &comp->cymbals.hh_verb_R);
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
        peq_gui("##ride_peq", comp->cymbals.ride_peq);
        ImGui::EndTable();
    }
    ImGui::PopID();
}

void view_lead_a(const char* id, composition* comp) {
    ImGui::PushID(id);
    ImGui::PopID();
}

void view_lead_b(const char* id, composition* comp) {
    ImGui::PushID(id);
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

void view_pad(const char* id, composition* comp) {
    ImGui::PushID(id);
    fdn_component("fdn", &comp->synth.pad_verb_L, &comp->synth.pad_verb_R);
    if (ImGui::BeginTable("##peq_table", 2)) {
        ImGui::TableNextColumn();
        ImGui::Text("pad preverb peq");
        peq_gui("##preverb_peq", comp->synth.pad_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("pad out peq");
        peq_gui("##postverb_peq", comp->synth.pad_peq);

        ImGui::EndTable();
    }
    ImGui::PopID();
}
