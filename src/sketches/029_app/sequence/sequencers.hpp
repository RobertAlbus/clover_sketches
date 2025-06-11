#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/cymbal/cymbal_024.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "lib/logging/logger.hpp"
#include "lib/sq/frsq_024.hpp"

#include "infrastructure/bar_grid.hpp"

#include "graph/graph.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"

struct sequencers {
    patterns patterns;
    arrangement arrangement;

    log_bus_000& log;
    bar_grid& grid;
    signal_graph& graph;

    frsq_024<log_bus_000, event> frsq_arrangement_print;

    frsq_024<kick_drum_000, event> frsq_kick;
    frsq_024<frsq_024<kick_drum_000, event>, event_meta_sq> meta_frsq_kick;

    frsq_024<cymbal_024, event> frsq_ride;
    frsq_024<frsq_024<cymbal_024, event>, event_meta_sq> meta_frsq_ride;

    frsq_024<subtractive_synth_000, event_midi> frsq_chord;
    frsq_024<frsq_024<subtractive_synth_000, event_midi>, event_meta_sq> meta_frsq_chord;

    sequencers(signal_graph& graph, bar_grid& grid, log_bus_000& log);
    void tick();

    bool is_playing = false;
    void play_from_bar(double bar);
    void play();
    void stop();

    void set_up();
    void set_up_kick(signal_graph& graph);
    void set_up_ride(signal_graph& graph);
    void set_up_chord(signal_graph& graph);
    void set_up_arrangement_print(signal_graph& graph);
};
