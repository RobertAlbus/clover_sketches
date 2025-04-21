#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "infrastructure/bar_grid.hpp"
#include "infrastructure/logger.hpp"

#include "composition/graph.hpp"
#include "instruments/frsq.hpp"
#include "instruments/kick.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/event.hpp"

struct sequencers {
    logger* log;
    bar_grid& grid;

    // need a placeholder for the voices, so reusing the event
    // this is fine because there is no need for a voice - will just println in the callback.
    frsq<event, event> frsq_arrangement_print;

    // pattern sequencers
    frsq<kick_drum, event> frsq_kick;
    frsq<subtractive_synth, event_midi> frsq_chord;

    // meta sequencers
    frsq<frsq<kick_drum, event>, event_meta_sq> meta_frsq_kick;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_chord;

    sequencers(graph& graph, bar_grid& grid);
    sequencers(graph& graph, bar_grid& grid, logger* log);
    void tick();

    void set_up_kick(graph& graph);
    void set_up_chord(graph& graph);
    void set_up_meta_sq(graph& graph);
    void set_up_arrangement_print(graph& graph);
};