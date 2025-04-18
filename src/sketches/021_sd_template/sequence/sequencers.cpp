// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <print>

#include "composition/composition.hpp"
#include "instruments/kick.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include "sequencers.hpp"

sequencers::sequencers(composition& comp) {
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
    frsq_kick.voices            = std::span<kick_drum>(&comp.kick.drum, 1);
    frsq_kick.duration_absolute = comp.sp_bar;
    frsq_kick.duration_relative = 4.;
    // frsq_kick.set_pattern(drum_patterns.patterns_kick[active_scene["kick"]]);

    frsq_kick.callback_start = [](kick_drum& voice, const event& data) { voice.key_on(); };
    frsq_kick.callback_end   = [](kick_drum& voice) { voice.key_off(); };
}

void sequencers::set_up_chord(composition& comp) {
    frsq_chord.voices = std::span<subtractive_synth>(comp.synth.chord.begin(), comp.synth.chord.end());
    frsq_chord.duration_absolute = comp.sp_bar;
    frsq_chord.duration_relative = 4.;
    // frsq_chord.set_pattern(synth_patterns.patterns_chord[active_scene["chord"]]);

    frsq_chord.callback_start = [](subtractive_synth& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_chord.callback_end = [](subtractive_synth& voice) { voice.key_off(); };
}

void sequencers::set_up_meta_sq(composition& comp) {
    meta_frsq_kick.voices  = std::span(&frsq_kick, 1);
    meta_frsq_chord.voices = std::span(&frsq_chord, 1);

    meta_frsq_kick.duration_absolute  = comp.sp_bar * comp.duration_bars;
    meta_frsq_chord.duration_absolute = comp.sp_bar * comp.duration_bars;

    meta_frsq_kick.duration_relative  = comp.duration_bars;
    meta_frsq_chord.duration_relative = comp.duration_bars;

    meta_frsq_kick.callback_start = [&](frsq<kick_drum, event>& voice, const event_meta_sq& event) {
        std::println(" - frsq_kick:    {} @ {}", event.pattern_index, event.start_time);
        voice.set_pattern(pattern::kick[event.pattern_index]);
    };
    meta_frsq_chord.callback_start = [&](frsq<subtractive_synth, event_midi>& voice,
                                         const event_meta_sq& event) {
        std::println(" - frsq_chord:   {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(pattern::chord[event.pattern_index]);
    };

    meta_frsq_kick.set_pattern(arrangement::kick, arrangement::playback_start);
    meta_frsq_chord.set_pattern(arrangement::chord, arrangement::playback_start);
}

void sequencers::set_up_arrangement_print(composition& comp) {
    frsq_arrangement_print.voices            = std::span(arrangement::bar.begin(), 1);
    frsq_arrangement_print.duration_absolute = comp.sp_bar * comp.duration_bars;
    frsq_arrangement_print.duration_relative = comp.duration_bars;
    frsq_arrangement_print.set_pattern(arrangement::bar);
    frsq_arrangement_print.callback_start = [](event& voice, const event& event) {
        std::println("--------");
        std::println("B: {}", event.start_time);
    };
}
