// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "lib/cymbal/cymbal_024.hpp"
#include "lib/kick_drum/kick_drum.hpp"

#include "graph/graph.hpp"
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
    set_up_ride(graph);
    set_up_chord(graph);
    set_up_arrangement_print(graph);
    set_up_meta_sq(graph);
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
    play_from_bar(0);
}

void sequencers::play_from_bar(double start_bar) {
    double duration_bars            = grid.duration_bars;
    double duration_bars_in_samples = grid.bars_to_samples(duration_bars);

    frsq_arrangement_print.set_pattern(arrangement.bar, duration_bars_in_samples, duration_bars, start_bar);
    meta_frsq_kick.set_pattern(arrangement.kick, duration_bars_in_samples, duration_bars, start_bar);
    meta_frsq_ride.set_pattern(arrangement.ride, duration_bars_in_samples, duration_bars, start_bar);
    meta_frsq_chord.set_pattern(arrangement.chord, duration_bars_in_samples, duration_bars, start_bar);

    frsq_arrangement_print.trigger_most_recent_event();
    meta_frsq_kick.trigger_most_recent_event();
    meta_frsq_ride.trigger_most_recent_event();
    meta_frsq_chord.trigger_most_recent_event();

    is_playing = true;
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

void sequencers::set_up_kick(signal_graph& graph) {
    frsq_kick.voices         = std::span<kick_drum_000>(&graph.kick, 1);
    frsq_kick.callback_start = [](kick_drum_000& voice, const event& data) { voice.key_on(); };
    frsq_kick.callback_end   = [](kick_drum_000& voice) { voice.key_off(); };
}

void sequencers::set_up_ride(signal_graph& graph) {
    frsq_ride.voices         = std::span<cymbal_024>(&graph.ride, 1);
    frsq_ride.callback_start = [](cymbal_024& voice, const event& data) { voice.key_on(); };
    frsq_ride.callback_end   = [](cymbal_024& voice) { voice.key_off(); };
}

void sequencers::set_up_chord(signal_graph& graph) {
    frsq_chord.voices         = std::span<subtractive_synth_000>(graph.chord.begin(), graph.chord.end());
    frsq_chord.callback_start = [](subtractive_synth_000& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_chord.callback_end = [](subtractive_synth_000& voice) { voice.key_off(); };
}

void sequencers::set_up_meta_sq(signal_graph& graph) {
    meta_frsq_kick.voices  = std::span(&frsq_kick, 1);
    meta_frsq_ride.voices  = std::span(&frsq_ride, 1);
    meta_frsq_chord.voices = std::span(&frsq_chord, 1);

    double duration_bars = grid.duration_bars;

    meta_frsq_kick.callback_start = arrangement_callback_for<kick_drum_000, event>(
            meta_frsq_kick, log, grid, patterns.kick, "frsq_kick");

    meta_frsq_ride.callback_start = arrangement_callback_for<cymbal_024, event>(
            meta_frsq_ride, log, grid, patterns.ride, "frsq_ride");

    meta_frsq_chord.callback_start = arrangement_callback_for<subtractive_synth_000, event_midi>(
            meta_frsq_chord, log, grid, patterns.chord, "frsq_chord");
}

void sequencers::set_up_arrangement_print(signal_graph& graph) {
    frsq_arrangement_print.voices            = std::span(arrangement.bar.begin(), 1);
    frsq_arrangement_print.duration_absolute = grid.bars_to_samples(grid.duration_bars);
    frsq_arrangement_print.duration_relative = grid.duration_bars;

    frsq_arrangement_print.callback_start = [&](event& voice, const event& event) {
        log_message_000 msg;
        snprintf(msg.text, sizeof(msg.text), "\n--------\n bar: %d", int(event.start_time));
        log.gui.try_enqueue(msg);
    };
}
