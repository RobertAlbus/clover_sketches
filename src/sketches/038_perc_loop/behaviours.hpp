#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/io/audio_callback.hpp"

#include "lib/logging/logger.hpp"
#include "lib/sq/bar_grid_029.hpp"

#include "graph/graph.hpp"
#include "gui/view.hpp"
#include "infra/behaviours_base.hpp"
#include "sequence/sequencers.hpp"

std::vector<frsq_pair> build_frsq_pairs(
    signal_graph& graph, bar_grid_029& grid, log_bus_000& log, patterns& patterns, arrangement& arrangement);

struct behaviours : public behaviour_base {
    behaviours();

    ~behaviours() override = default;

    // lol (160.000001 bpm floating point math and chord pattern: mortal enemies)
    float bpm                  = 134;
    double duration_bars       = 32;
    double render_repeat_count = 2;
    int channel_count          = 2;
    float sample_rate          = 48000;

    bar_grid_029 grid{fs(), bpm, duration_bars};
    sequencers sequencers{grid, logger};
    log_bus_000 logger;

    patterns patterns;
    arrangement arrangement;
    signal_graph graph{fs(), bpm};
    view view{project_name(), sequencers, graph, logger};

    float fs() override;

    int channel_count_out() override;
    bool view_draw() override;
    void view_init() override;
    void view_deinit() override;
    int render_duration_samples() override;
    int render_runout_duration_samples() override;
    void sequencer_start() override;
    void sequencer_stop() override;
    clover::io::callback create_audio_callback() override;

    constexpr std::string project_name() override {
        return PROJECT_NAME_STR;
    }
};
