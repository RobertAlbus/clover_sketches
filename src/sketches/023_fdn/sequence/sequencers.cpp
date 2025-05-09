// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "graph/graph.hpp"
#include "lib/kick_drum/kick_drum.hpp"

#include "sequence/arrangement_callback_builder.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include "sequencers.hpp"

sequencers::sequencers(signal_graph& graph, bar_grid& grid, log_bus_000& log)
    : log{log}, grid{grid}, graph{graph} {
    set_up();
}

void sequencers::set_up() {
    set_up_kick(graph);
    set_up_arrangement_print(graph);
    set_up_meta_sq(graph);
}
void sequencers::tick() {
    frsq_arrangement_print.tick();
    meta_frsq_kick.tick();
    frsq_kick.tick();
}

void sequencers::set_up_kick(signal_graph& graph) {
    frsq_kick.voices         = std::span<kick_drum_000>(&graph.kick, 1);
    frsq_kick.callback_start = [](kick_drum_000& voice, const event& data) { voice.key_on(); };
    frsq_kick.callback_end   = [](kick_drum_000& voice) { voice.key_off(); };
}

void sequencers::set_up_meta_sq(signal_graph& graph) {
    meta_frsq_kick.voices = std::span(&frsq_kick, 1);

    meta_frsq_kick.duration_absolute = grid.bars_to_samples(grid.duration_bars);

    meta_frsq_kick.duration_relative = grid.duration_bars;

    meta_frsq_kick.callback_start = callback_for<kick_drum_000, event>(log, grid, pattern::kick, "frsq_kick");

    meta_frsq_kick.set_pattern(
            arrangement::kick,
            grid.bars_to_samples(grid.duration_bars),
            grid.duration_bars,
            arrangement::playback_start);
}

void sequencers::set_up_arrangement_print(signal_graph& graph) {
    frsq_arrangement_print.voices            = std::span(arrangement::bar.begin(), 1);
    frsq_arrangement_print.duration_absolute = grid.bars_to_samples(grid.duration_bars);
    frsq_arrangement_print.duration_relative = grid.duration_bars;
    frsq_arrangement_print.set_pattern(
            arrangement::bar, grid.bars_to_samples(grid.duration_bars), grid.duration_bars);
    frsq_arrangement_print.callback_start = [&](event& voice, const event& event) {
        log_message_000 msg;
        snprintf(msg.text, sizeof(msg.text), "\n--------\n bar: %d", int(event.start_time));
        log.gui.try_enqueue(msg);
    };
}
