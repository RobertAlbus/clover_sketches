#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/logging/logger.hpp"
#include "lib/sq/frsq_024.hpp"

#include "lib/sq/bar_grid_029.hpp"

#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include <functional>
#include <memory>

struct frsq_pair {
    std::unique_ptr<frsq_base_024> sq;
    std::unique_ptr<frsq_base_024> meta_sq;
};

struct sequencers {
    sequencers(bar_grid_029& grid, log_bus_000& log);
    sequencers(std::vector<frsq_pair>&& new_frsq_pairs, bar_grid_029& grid, log_bus_000& log);
    void set_frsq_pairs(std::vector<frsq_pair>&& new_frsq_pairs);

    patterns patterns;
    arrangement arrangement;

    bar_grid_029& grid;

    frsq_024<log_bus_000, event> frsq_arrangement_print;

    std::vector<frsq_pair> frsq_pairs;
    std::function<void(double start_bar)> on_play = [](double start_bar) {};
    std::function<void()> on_stop                 = []() {};

    void tick();

    bool is_playing = false;
    void play_from_bar(double bar);
    void play();
    void stop();

    void set_up_arrangement_print(log_bus_000& log);
};
