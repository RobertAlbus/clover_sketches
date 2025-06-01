// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <complex>
#include <print>
#include <ranges>

#include "graph/graph.hpp"
#include "gui/components/cymbal_000.hpp"
#include "imgui.h"
#include "implot.h"

#include "lib/fdn/draw_fdn8_023.hpp"
#include "lib/kick_drum/draw_kick_drum.hpp"
#include "lib/logging/draw_gui_log_canvas.hpp"
#include "lib/mixer/draw_mixer.hpp"
#include "lib/peq/draw_peq.hpp"

#include "util/filter_response.hpp"

#include "controllers.hpp"

void controller_mixer(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(id);
    static log_canvas_000 canvas;
    draw_gui_log_canvas_000("log_canvas", canvas, logger, nullptr);

    static double x = 0.5f;
    static double y = 0.5f;

    static bool show_plot = true;
    static std::vector<float> freqs(2000, 0);
    static complex_response response_cplx_0(2000, 0);
    static complex_response response_cplx_1(2000, 0);
    static complex_response response_cplx_2(2000, 0);
    static complex_response response_cplx_3(2000, 0);
    static complex_response response_cplx_all(2000, 0);
    static computed_response response_cmpt{2000};

    static bool init_response = true;
    if (init_response) {
        log_spaced_freqs(freqs, 20, 24000);
        compute_response(graph.main_eq.filters[0].m_coeffs, response_cplx_0, freqs);
        compute_response(graph.main_eq.filters[1].m_coeffs, response_cplx_1, freqs);
        compute_response(graph.main_eq.filters[2].m_coeffs, response_cplx_2, freqs);
        compute_response(graph.main_eq.filters[3].m_coeffs, response_cplx_3, freqs);

        for (auto [cr0, cr1, cr2, cr3, cra] : std::views::zip(
                     response_cplx_0, response_cplx_1, response_cplx_2, response_cplx_3, response_cplx_all)) {
            cra = cr0 * cr1 * cr2 * cr3;
        }
        compute_spectrum(response_cplx_all, response_cmpt);
        unwrap_phase(response_cmpt.angles);
        init_response = false;
    }
    ImGui::Checkbox("show plot", &show_plot);
    if (show_plot) {
        if (ImPlot::BeginPlot("Draggable Point")) {
            ImPlot::PlotLine("magnitude", freqs.data(), response_cmpt.magnitudes.data(), (int)freqs.size());
            ImPlot::PlotLine("phase", freqs.data(), response_cmpt.angles.data(), (int)freqs.size());
            ImPlot::DragPoint(0, &x, &y, ImVec4(1, 0, 0, 1), 4, ImPlotDragToolFlags_None);
            ImPlot::EndPlot();
        }
    }

    draw_mixer_000("new_mix", &graph.mixer_tracks);
    draw_peq_000("##master_peq", graph.main_eq);

    ImGui::PopID();
}

void controller_kick(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(id);

    draw_kick_drum_000("kick_synth", graph.kick);
    if (ImGui::BeginTable("##kick_table", 2)) {
        ImGui::TableNextColumn();
        draw_peq_000("##kick_preverb_peq", graph.kick_preverb_peq);
        ImGui::TableNextColumn();
        ImGui::Text("kick postverb peq");
        draw_peq_000("##kick_out_peq", graph.kick_out_peq);
        ImGui::EndTable();
    }
    draw_fdn8_023("##kick_fdn", &graph.kick_verb, nullptr);

    ImGui::PopID();
}

void controller_ride(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(id);

    draw_cymbal_000("ride", graph.ride);
    draw_peq_000("##ride_eq", graph.ride_peq);

    ImGui::PopID();
}

void controller_chord(const char* id, signal_graph& graph, log_bus_000& logger) {
    ImGui::PushID(id);
    draw_fdn8_023("fdn", &graph.chord_verb_L, &graph.chord_verb_R);
    if (ImGui::BeginTable("##peq_table", 2)) {
        ImGui::TableNextColumn();
        ImGui::Text("chord preverb peq");
        draw_peq_000("##preverb_peq", graph.chord_preverb_peq);

        ImGui::TableNextColumn();
        ImGui::Text("chord out peq");
        draw_peq_000("##postverb_peq", graph.chord_peq);

        ImGui::EndTable();
    }
    ImGui::PopID();
}
