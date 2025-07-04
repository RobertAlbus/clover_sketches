// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "implot.h"

#include "lib/cymbal/cymbal_024.hpp"
#include "lib/logging/logger.hpp"
#include "lib/sq/bar_grid_029.hpp"

#include "graph/graph.hpp"
#include "sequence/sequencers.hpp"
#include "sequence/set_up_sequencing.hpp"

#include "behaviours.hpp"

std::vector<frsq_pair> build_frsq_pairs(
        signal_graph& graph,
        bar_grid_029& grid,
        log_bus_000& log,
        patterns& patterns,
        arrangement& arrangement) {
    std::vector<frsq_pair> frsq_pairs;
    frsq_pairs.emplace_back(create_sequencers(
            std::span<kick_drum_000>(&graph.kick, 1),
            [](kick_drum_000& voice, const event& data) { voice.key_on(); },
            [](kick_drum_000& voice) { voice.key_off(); },
            log,
            grid,
            patterns.kick,
            arrangement.kick,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_kick"));
    frsq_pairs.emplace_back(create_sequencers(
            std::span<cymbal_024>(&graph.snare_impulse, 1),
            [](cymbal_024& voice, const event& data) {
                for (auto& osc : voice.oscs)
                    osc.phase(0);
                voice.key_on();
            },
            [](cymbal_024& voice) { voice.key_off(); },
            log,
            grid,
            patterns.snare,
            arrangement.snare,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_snare"));
    frsq_pairs.emplace_back(create_sequencers(
            std::span<cymbal_024>(&graph.ride, 1),
            [](cymbal_024& voice, const event& data) { voice.key_on(); },
            [](cymbal_024& voice) { voice.key_off(); },
            log,
            grid,
            patterns.ride,
            arrangement.ride,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_ride"));
    frsq_pairs.emplace_back(create_sequencers(
            std::span<subtractive_synth_000>(graph.chord),
            [](subtractive_synth_000& voice, const event_midi& data) { voice.key_on(data.note); },
            [](subtractive_synth_000& voice) { voice.key_off(); },
            log,
            grid,
            patterns.chord,
            arrangement.chord,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_chord"));

    return frsq_pairs;
}

behaviours::behaviours() {
    sequencers.set_frsq_pairs(build_frsq_pairs(graph, grid, logger, patterns, arrangement));
}

float behaviours::fs() {
    return sample_rate;
}

int behaviours::channel_count_out() {
    return channel_count;
}

bool behaviours::view_draw() {
    return view.draw();
}

void behaviours::view_init() {
    ImPlot::CreateContext();
    ImPlot::GetInputMap().ZoomMod     = ImGuiMod_Ctrl;
    ImPlot::GetInputMap().SelectMod   = ImGuiMod_Ctrl;
    ImPlot::GetInputMap().PanMod      = ImGuiMod_Ctrl;
    ImPlot::GetInputMap().OverrideMod = ImGuiMod_Shift;

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    ImGuiStyle& style      = ImGui::GetStyle();
    style.WindowPadding    = ImVec2(10.0f, 10.0f);
    style.WindowBorderSize = 0.0f;
    style.WindowRounding   = 0.0f;
}
void behaviours::view_deinit() {
    ImPlot::DestroyContext();
}

int behaviours::render_duration_samples() {
    return int(render_repeat_count * grid.duration_samples());
}

int behaviours::render_runout_duration_samples() {
    return int(2.f * grid.bars_to_samples(4)) + 1;
}

void behaviours::sequencer_start() {
    sequencers.play();
}

void behaviours::sequencer_stop() {
    sequencers.stop();
}

clover::io::callback behaviours::create_audio_callback() {
    return [&](clover::io::callback_args data) {
        float& L = *(data.output);
        float& R = *(data.output + 1);

        sequencers.tick();
        std::tie(L, R) = graph.tick();

        return clover::io::callback_status::cont;
    };
}