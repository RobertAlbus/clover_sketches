#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "implot.h"

#include "clover/io/audio_callback.hpp"

#include "lib/logging/logger.hpp"
#include "lib/sq/bar_grid_029.hpp"

#include "graph/graph.hpp"
#include "gui/view.hpp"
#include "infra/behaviours_base.hpp"
#include "sequence/sequencers.hpp"

std::vector<frsq_pair> build_frsq_pairs(
        signal_graph& graph,
        bar_grid_029& grid,
        log_bus_000& log,
        patterns& patterns,
        arrangement& arrangement);

struct behaviours : public behaviour_base {
    behaviours() {
        sequencers.set_frsq_pairs(build_frsq_pairs(graph, grid, logger, patterns, arrangement));
    }

    ~behaviours() override = default;

    // lol (160.000001 bpm floating point math and chord pattern: mortal enemies)
    float bpm                  = 160.00001;
    double duration_bars       = 32;
    double render_repeat_count = 2;

    bar_grid_029 grid{fs(), bpm, duration_bars};
    sequencers sequencers{grid, logger};
    log_bus_000 logger;

    patterns patterns;
    arrangement arrangement;
    signal_graph graph{fs()};
    view view{sequencers, graph, logger};

    float fs() override {
        return 48000;
    }

    int channel_count_out() override {
        return 2;
    }

    bool view_draw() override {
        return view.draw();
    }

    void view_init() override {
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
    void view_deinit() override {
        ImPlot::DestroyContext();
    }

    int render_duration_samples() override {
        return int(render_repeat_count * grid.duration_samples());
    }

    int render_runout_duration_samples() override {
        return int(2.f * grid.bars_to_samples(4)) + 1;
    }

    void sequencer_start() override {
        sequencers.play();
    }

    void sequencer_stop() override {
        sequencers.stop();
    }

    constexpr std::string project_name() override {
        return "032_meter";
    }

    clover::io::callback create_audio_callback() override {
        return [&](clover::io::callback_args data) {
            float& L = *(data.output);
            float& R = *(data.output + 1);

            sequencers.tick();
            std::tie(L, R) = graph.tick();

            return clover::io::callback_status::cont;
        };
    }
};
