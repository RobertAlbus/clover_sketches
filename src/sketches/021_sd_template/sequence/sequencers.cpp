// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "composition/composition.hpp"
#include "instruments/kick.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/arrangement_callback_builder.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include "sequencers.hpp"

sequencers::sequencers(composition& comp, bar_grid& grid) : sequencers(comp, grid, nullptr) {
}
sequencers::sequencers(composition& comp, bar_grid& grid, logger* log) : grid{grid}, log{log} {
    set_up_kick(comp);
    set_up_chord(comp);
    set_up_arrangement_print(comp);
    set_up_meta_sq(comp);
}

void sequencers::tick() {
    frsq_arrangement_print.tick();
    meta_frsq_kick.tick();
    meta_frsq_chord.tick();

    frsq_kick.tick();
    frsq_chord.tick();
}

void sequencers::set_up_kick(composition& comp) {
    frsq_kick.voices = std::span<kick_drum>(&comp.kick.drum, 1);
    // frsq_kick.duration_absolute = comp.sp_bar;
    // frsq_kick.duration_relative = 4.;
    // frsq_kick.set_pattern(drum_patterns.patterns_kick[active_scene["kick"]]);

    frsq_kick.callback_start = [](kick_drum& voice, const event& data) { voice.key_on(); };
    frsq_kick.callback_end   = [](kick_drum& voice) { voice.key_off(); };
}

void sequencers::set_up_chord(composition& comp) {
    frsq_chord.voices = std::span<subtractive_synth>(comp.synth.chord.begin(), comp.synth.chord.end());
    // frsq_chord.duration_absolute = comp.sp_bar;
    // frsq_chord.duration_relative = 4.;
    // frsq_chord.set_pattern(synth_patterns.patterns_chord[active_scene["chord"]]);

    frsq_chord.callback_start = [](subtractive_synth& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_chord.callback_end = [](subtractive_synth& voice) { voice.key_off(); };
}

void sequencers::set_up_meta_sq(composition& comp) {
    meta_frsq_kick.voices  = std::span(&frsq_kick, 1);
    meta_frsq_chord.voices = std::span(&frsq_chord, 1);

    meta_frsq_kick.duration_absolute  = grid.bars_to_samples(comp.duration_bars);
    meta_frsq_chord.duration_absolute = grid.bars_to_samples(comp.duration_bars);

    meta_frsq_kick.duration_relative  = comp.duration_bars;
    meta_frsq_chord.duration_relative = comp.duration_bars;

    meta_frsq_kick.callback_start = callback_for<kick_drum, event>(&log, grid, pattern::kick, "frsq_kick");

    meta_frsq_chord.callback_start =
            callback_for<subtractive_synth, event_midi>(&log, grid, pattern::chord, "frsq_chord");

    meta_frsq_kick.set_pattern(
            arrangement::kick,
            grid.bars_to_samples(comp.duration_bars),
            comp.duration_bars,
            arrangement::playback_start);
    meta_frsq_chord.set_pattern(
            arrangement::chord,
            grid.bars_to_samples(comp.duration_bars),
            comp.duration_bars,
            arrangement::playback_start);
}

void sequencers::set_up_arrangement_print(composition& comp) {
    frsq_arrangement_print.voices            = std::span(arrangement::bar.begin(), 1);
    frsq_arrangement_print.duration_absolute = comp.sp_bar * comp.duration_bars;
    frsq_arrangement_print.duration_relative = comp.duration_bars;
    frsq_arrangement_print.set_pattern(
            arrangement::bar, grid.bars_to_samples(comp.duration_bars), comp.duration_bars);
    frsq_arrangement_print.callback_start = [&](event& voice, const event& event) {
        if (log) {
            gui_log_message msg;
            snprintf(msg.text, sizeof(msg.text), "\n--------\n bar: %d", int(event.start_time));
            bool sent = log->gui.try_enqueue(msg);
            if (!sent) {
                std::println("meta_frsq_chord.callback_start failed to log to gui");
            }
        }
    };
}
