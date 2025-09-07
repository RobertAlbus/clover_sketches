// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "sequencers.hpp"
#include "lib/sq/frsq_024.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"

sequencers::sequencers(bar_grid_029& grid, log_bus_000& log) : sequencers({}, grid, log) {
}
sequencers::sequencers(std::vector<frsq_pair>&& new_frsq_pairs, bar_grid_029& grid, log_bus_000& log)
    : grid{grid}, frsq_pairs{std::move(new_frsq_pairs)} {
    set_up_arrangement_print(log);
}

void sequencers::set_frsq_pairs(std::vector<frsq_pair>&& new_frsq_pairs) {
    frsq_pairs = std::move(new_frsq_pairs);
}

void sequencers::tick() {
    if (!is_playing)
        return;

    frsq_arrangement_print.tick();

    for (auto& frsq_pair : frsq_pairs) {
        frsq_pair.meta_sq->tick();
        frsq_pair.sq->tick();
    }
}

void sequencers::play() {
    is_playing = true;
}

void sequencers::play_from_bar(double start_bar) {
    double duration_bars            = grid.duration_bars;
    double duration_bars_in_samples = grid.bars_to_samples(duration_bars);

    frsq_arrangement_print.set_time(start_bar);

    for (auto& frsq_pair : frsq_pairs) {
        frsq_pair.meta_sq->set_time(start_bar);
    }
    is_playing = true;

    if (start_bar == 0)
        return;

    frsq_arrangement_print.trigger_most_recent_event();
    for (auto& frsq_pair : frsq_pairs) {
        frsq_pair.meta_sq->trigger_most_recent_event();
    }
}

void sequencers::stop() {
    frsq_arrangement_print.choke_all();
    for (auto& frsq_pair : frsq_pairs) {
        frsq_pair.sq->choke_all();
        frsq_pair.meta_sq->choke_all();
    }

    is_playing = false;
}

void sequencers::set_up_arrangement_print(log_bus_000& log) {
    frsq_arrangement_print.voices            = std::span(&log, 1);
    frsq_arrangement_print.duration_absolute = grid.bars_to_samples(grid.duration_bars);
    frsq_arrangement_print.duration_relative = grid.duration_bars;

    frsq_arrangement_print.callback_start = [&](log_bus_000& voice, const event& event) {
        log_message_000 msg;
        snprintf(msg.text, sizeof(msg.text), "\n--------\n bar: %d", int(event.start_time));
        voice.gui.try_enqueue(msg);
    };

    double duration_bars            = grid.duration_bars;
    double duration_bars_in_samples = grid.bars_to_samples(duration_bars);
    frsq_arrangement_print.set_pattern(arrangement.bar, duration_bars_in_samples, duration_bars, 0);
}
