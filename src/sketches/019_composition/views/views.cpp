// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "lib/fdn/draw_fdn.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/peq/draw_peq.hpp"

#include "views.hpp"
#include "visual_components/mixer_ui.hpp"

void controller_mixer(const char* id, composition* comp) {
    ImGui::PushID(id);

    mixer_component("mix", &comp->mix);
    draw_peq_000("##master_peq", comp->master_peq);

    ImGui::PopID();
}

void controller_kick(const char* id, composition* comp) {
    ImGui::PushID(id);

    draw_kick_drum_000("kick_synth", comp->kick.drum);
    if (ImGui::BeginTable("##kick_table", 2)) {
        ImGui::TableNextColumn();
        draw_peq_000("##kick_preverb_peq", comp->kick.preverb_peq);
        ImGui::TableNextColumn();
        ImGui::Text("kick postverb peq");
        draw_peq_000("##kick_out_peq", comp->kick.out_peq);
        ImGui::EndTable();
    }
    fdn_component("##kick_fdn", &comp->kick.verb, nullptr);

    ImGui::PopID();
}

void view_bass(const char* id, composition* comp) {
    ImGui::PushID(id);

    draw_peq_000("##bass_out_peq", comp->bass.out_peq);

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
        draw_peq_000("##hh_preverb_peq", comp->cymbals.hh_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("hh out peq");
        draw_peq_000("##hh_peq", comp->cymbals.hh_peq);

        ImGui::TableNextColumn();
        ImGui::Text("ride peq");
        draw_peq_000("##ride_peq", comp->cymbals.ride_peq);
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

void controller_chord(const char* id, composition* comp) {
    ImGui::PushID(id);
    fdn_component("fdn", &comp->synth.chord_verb_L, &comp->synth.chord_verb_R);
    if (ImGui::BeginTable("##peq_table", 2)) {
        ImGui::TableNextColumn();
        ImGui::Text("chord preverb peq");
        draw_peq_000("##preverb_peq", comp->synth.chord_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("chord out peq");
        draw_peq_000("##postverb_peq", comp->synth.chord_peq);

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
        draw_peq_000("##preverb_peq", comp->synth.pad_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("pad out peq");
        draw_peq_000("##postverb_peq", comp->synth.pad_peq);

        ImGui::EndTable();
    }
    ImGui::PopID();
}
