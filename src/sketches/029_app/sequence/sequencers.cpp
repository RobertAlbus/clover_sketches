// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "lib/cymbal/cymbal_024.hpp"
#include "lib/kick_drum/kick_drum.hpp"

#include "graph/graph.hpp"
#include "lib/sq/frsq_024.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include "sequence/set_up_sequencing.hpp"
#include "sequencers.hpp"

sequencers::sequencers(signal_graph& graph, bar_grid& grid, log_bus_000& log) : grid{grid} {
    set_up(graph, log);
}

void sequencers::set_up(signal_graph& graph, log_bus_000& log) {
    set_up_kick(graph, log);
    set_up_ride(graph, log);
    set_up_chord(graph, log);
    set_up_arrangement_print(log);
}

void sequencers::tick() {
    if (!is_playing)
        return;
    frsq_arrangement_print.tick();
    meta_frsq_kick.tick();
    meta_frsq_ride.tick();
    meta_frsq_chord.tick();
    frsq_kick.tick();
    frsq_ride.tick();
    frsq_chord.tick();
}

void sequencers::play() {
    is_playing = true;
}

void sequencers::play_from_bar(double start_bar) {
    double duration_bars            = grid.duration_bars;
    double duration_bars_in_samples = grid.bars_to_samples(duration_bars);

    frsq_arrangement_print.set_time(start_bar);
    meta_frsq_kick.set_time(start_bar);
    meta_frsq_ride.set_time(start_bar);
    meta_frsq_chord.set_time(start_bar);
    is_playing = true;

    if (start_bar == 0)
        return;

    frsq_arrangement_print.trigger_most_recent_event();
    meta_frsq_kick.trigger_most_recent_event();
    meta_frsq_ride.trigger_most_recent_event();
    meta_frsq_chord.trigger_most_recent_event();
}

void sequencers::stop() {
    frsq_arrangement_print.choke_all();
    meta_frsq_kick.choke_all();
    meta_frsq_ride.choke_all();
    meta_frsq_chord.choke_all();
    frsq_kick.choke_all();
    frsq_ride.choke_all();
    frsq_chord.choke_all();

    is_playing = false;
}

void sequencers::set_up_kick(signal_graph& graph, log_bus_000& log) {
    set_up_sequencing(
            std::span<kick_drum_000>(&graph.kick, 1),
            frsq_kick,
            [](kick_drum_000& voice, const event& data) { voice.key_on(); },
            [](kick_drum_000& voice) { voice.key_off(); },
            meta_frsq_kick,
            log,
            grid,
            patterns.kick,
            arrangement.kick,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_kick");
}

void sequencers::set_up_ride(signal_graph& graph, log_bus_000& log) {
    set_up_sequencing(
            std::span<cymbal_024>(&graph.ride, 1),
            frsq_ride,
            [](cymbal_024& voice, const event& data) { voice.key_on(); },
            [](cymbal_024& voice) { voice.key_off(); },
            meta_frsq_ride,
            log,
            grid,
            patterns.ride,
            arrangement.ride,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_ride");
}

void sequencers::set_up_chord(signal_graph& graph, log_bus_000& log) {
    set_up_sequencing(
            std::span<subtractive_synth_000>(graph.chord),
            frsq_chord,
            [](subtractive_synth_000& voice, const event_midi& data) { voice.key_on(data.note); },
            [](subtractive_synth_000& voice) { voice.key_off(); },
            meta_frsq_chord,
            log,
            grid,
            patterns.chord,
            arrangement.chord,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_chord");
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
