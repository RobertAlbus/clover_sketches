#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "infrastructure/bar_grid.hpp"
#include "infrastructure/logger.hpp"

#include "composition/graph.hpp"
#include "instruments/frsq.hpp"
#include "instruments/subtractive_synth.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "sequence/event.hpp"

struct sequencers {
    logger& log;
    bar_grid& grid;
    signal_graph& graph;

    // need a placeholder for the voices, so reusing the event
    // this is fine because there is no need for a voice - will just println in the callback.
    frsq<event, event> frsq_arrangement_print;

    // pattern sequencers
    frsq<kick_drum_000, event> frsq_kick;
    frsq<subtractive_synth, event_midi> frsq_chord;

    // meta sequencers
    frsq<frsq<kick_drum_000, event>, event_meta_sq> meta_frsq_kick;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_chord;

    sequencers(signal_graph& graph, bar_grid& grid, logger& log);
    void tick();

    void set_up();
    void set_up_kick(signal_graph& graph);
    void set_up_chord(signal_graph& graph);
    void set_up_meta_sq(signal_graph& graph);
    void set_up_arrangement_print(signal_graph& graph);
    void set_up_automation(signal_graph& graph);
};