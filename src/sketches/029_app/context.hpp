#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/io/audio_callback.hpp"
#include "lib/logging/logger.hpp"
#include "lib/sq/bar_grid_029.hpp"

#include "graph/graph.hpp"
#include "gui/view.hpp"
#include "sequence/sequencers.hpp"

std::vector<frsq_pair> build_frsq_pairs(
        signal_graph& graph,
        bar_grid_029& grid,
        log_bus_000& log,
        patterns& patterns,
        arrangement& arrangement);

struct context_base {
    // render behaviours
    virtual ~context_base()                      = default;
    virtual int render_duration_samples()        = 0;
    virtual int render_runout_duration_samples() = 0;
    virtual void sequencer_start()               = 0;
    virtual void sequencer_stop()                = 0;
    virtual constexpr std::string project_name() = 0;

    // live behaviours
    virtual bool draw_view()                             = 0;
    virtual clover::io::callback create_audio_callback() = 0;
    virtual int channel_count_out()                      = 0;
    virtual float fs()                                   = 0;
};

struct context_029 : public context_base {
    context_029() {
        sequencers.set_frsq_pairs(build_frsq_pairs(graph, grid, logger, patterns, arrangement));
    }

    ~context_029() override = default;

    float bpm                  = 124;
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

    bool draw_view() override {
        return view.draw();
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
        return "029_app";
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
