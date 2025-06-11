#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/logging/logger.hpp"
#include "lib/sq/frsq_024.hpp"

#include "infrastructure/bar_grid.hpp"

#include "graph/graph.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include <memory>

struct frsq_pair {
    std::unique_ptr<frsq_base_024> sq;
    std::unique_ptr<frsq_base_024> meta_sq;
};

struct sequencers {
    patterns patterns;
    arrangement arrangement;

    bar_grid& grid;

    frsq_024<log_bus_000, event> frsq_arrangement_print;

    std::vector<frsq_pair> frsq_pairs;

    sequencers(signal_graph& graph, bar_grid& grid, log_bus_000& log);
    void tick();

    bool is_playing = false;
    void play_from_bar(double bar);
    void play();
    void stop();

    void set_up(signal_graph& graph, log_bus_000& log);
    void set_up_kick(signal_graph& graph, log_bus_000& log);
    void set_up_ride(signal_graph& graph, log_bus_000& log);
    void set_up_chord(signal_graph& graph, log_bus_000& log);
    void set_up_arrangement_print(log_bus_000& log);
};
