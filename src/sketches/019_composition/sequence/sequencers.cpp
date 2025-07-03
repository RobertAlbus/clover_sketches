// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <map>
#include <print>

#include "lib/kick_drum/kick_drum.hpp"
#include "lib/subtractive_synth/subtractive_synth_000.hpp"

#include "composition/composition.hpp"
#include "sequence/event.hpp"
#include "sequence/pattern_meta.hpp"
#include "sequence/pattern_synth.hpp"
#include "sequencers.hpp"

std::map<const char*, size_t> scene_1 = {
    {"kick", 1},
    {"bass", 1},

    {"hh1", 1},
    {"hh2", 1},
    {"hh3", 1},
    {"ride", 1},

    {"chord", 1},
    {"pad", 1},
    {"lead_a1", 1},
    {"lead_a2", 0},
    {"lead_a3", 0},
    {"lead_b1", 1},
    {"lead_b2", 0},
    {"lead_b3", 0},
};

std::map<const char*, size_t> scene_2 = {
    {"kick", 1},
    {"bass", 1},

    {"hh1", 1},
    {"hh2", 1},
    {"hh3", 1},
    {"ride", 1},

    {"chord", 1},
    {"pad", 1},
    {"lead_a1", 0},
    {"lead_a2", 2},
    {"lead_b1", 0},
    {"lead_b2", 2},
};

std::map<const char*, size_t> scene_3 = {
    {"kick", 1},
    {"bass", 1},

    {"hh1", 1},
    {"hh2", 1},
    {"hh3", 1},
    {"ride", 1},

    {"chord", 1},
    {"pad", 1},
    {"lead_a1", 3},
    {"lead_a2", 0},
    {"lead_b1", 0},
    {"lead_b2", 0},
};

std::map<const char*, size_t>& active_scene = scene_2;

sequencers::sequencers(composition& comp) {
    set_up_kick(comp);
    set_up_bass(comp);

    set_up_hh1(comp);
    set_up_hh2(comp);
    set_up_hh3(comp);
    set_up_ride(comp);

    set_up_chord(comp);
    set_up_pad(comp);
    set_up_lead_a(comp);
    set_up_lead_b(comp);
    set_up_meta_sq(comp);
    set_up_arrangement_print(comp);
}

void sequencers::tick() {
    frsq_arrangement_print.tick();
    meta_frsq_kick.tick();
    meta_frsq_bass.tick();
    meta_frsq_hh1.tick();
    meta_frsq_hh2.tick();
    meta_frsq_hh3.tick();
    meta_frsq_ride.tick();
    meta_frsq_chord.tick();
    meta_frsq_lead_a1.tick();
    meta_frsq_lead_a2.tick();
    meta_frsq_lead_b1.tick();
    meta_frsq_lead_b2.tick();
    meta_frsq_pad.tick();

    frsq_kick.tick();
    frsq_bass.tick();
    frsq_hh1.tick();
    frsq_hh2.tick();
    frsq_hh3.tick();
    frsq_ride.tick();
    frsq_chord.tick();
    frsq_pad.tick();
    frsq_lead_a1.tick();
    frsq_lead_a2.tick();
    frsq_lead_b1.tick();
    frsq_lead_b2.tick();
}

void sequencers::set_up_kick(composition& comp) {
    frsq_kick.voices            = std::span<kick_drum_000>(&comp.kick.drum, 1);
    frsq_kick.duration_absolute = comp.bar;
    frsq_kick.duration_relative = 4.;
    // frsq_kick.set_pattern(drum_patterns.patterns_kick[active_scene["kick"]]);

    frsq_kick.callback_start = [](kick_drum_000& voice, const event& data) { voice.key_on(); };
    frsq_kick.callback_end   = [](kick_drum_000& voice) { voice.key_off(); };
}
void sequencers::set_up_bass(composition& comp) {
    frsq_bass.voices            = std::span<subtractive_synth_000>(&comp.bass.synth, 1);
    frsq_bass.duration_absolute = comp.beat * 4;
    frsq_bass.duration_relative = 8.;
    // frsq_bass.set_pattern(drum_patterns.patterns_bass[active_scene["bass"]]);

    frsq_bass.callback_start = [](subtractive_synth_000& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_bass.callback_end = [](subtractive_synth_000& voice) { voice.key_off(); };
}

void sequencers::set_up_hh1(composition& comp) {
    frsq_hh1.voices            = std::span<cymbal_000>(&comp.cymbals.hh1, 1);
    frsq_hh1.duration_absolute = comp.beat * 2;
    frsq_hh1.duration_relative = 2.;
    // frsq_hh1.set_pattern(drum_patterns.patterns_hh1[active_scene["hh1"]]);

    frsq_hh1.callback_start = [](cymbal_000& voice, const event& data) { voice.key_on(); };
    frsq_hh1.callback_end   = [](cymbal_000& voice) { voice.key_off(); };
}

void sequencers::set_up_hh2(composition& comp) {
    frsq_hh2.voices            = std::span<cymbal_000>(&comp.cymbals.hh2, 1);
    frsq_hh2.duration_absolute = comp.beat * 2;
    frsq_hh2.duration_relative = 2.;
    // frsq_hh2.set_pattern(drum_patterns.patterns_hh2[active_scene["hh2"]]);

    frsq_hh2.callback_start = [](cymbal_000& voice, const event& data) { voice.key_on(); };
    frsq_hh2.callback_end   = [](cymbal_000& voice) { voice.key_off(); };
}

void sequencers::set_up_hh3(composition& comp) {
    frsq_hh3.voices            = std::span<subtractive_synth_000>(&comp.cymbals.hh3, 1);
    frsq_hh3.duration_absolute = comp.beat * 8;
    frsq_hh3.duration_relative = 8.;
    // frsq_hh3.set_pattern(drum_patterns.patterns_hh3[active_scene["hh3"]]);

    frsq_hh3.callback_start = [](subtractive_synth_000& voice, const event& data) { voice.key_on(50); };
    frsq_hh3.callback_end   = [](subtractive_synth_000& voice) { voice.key_off(); };
}

void sequencers::set_up_ride(composition& comp) {
    frsq_ride.voices            = std::span<cymbal_000>(&comp.cymbals.ride, 1);
    frsq_ride.duration_absolute = comp.beat;
    frsq_ride.duration_relative = 1.;
    // frsq_ride.set_pattern(drum_patterns.patterns_ride[active_scene["ride"]]);

    frsq_ride.callback_start = [](cymbal_000& voice, const event& data) { voice.key_on(); };
    frsq_ride.callback_end   = [](cymbal_000& voice) { voice.key_off(); };
}

void sequencers::set_up_chord(composition& comp) {
    frsq_chord.voices = std::span<subtractive_synth_000>(comp.synth.chord.begin(), comp.synth.chord.end());
    frsq_chord.duration_absolute = comp.beat * 4;
    frsq_chord.duration_relative = 4.;
    // frsq_chord.set_pattern(synth_patterns.patterns_chord[active_scene["chord"]]);

    frsq_chord.callback_start = [](subtractive_synth_000& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_chord.callback_end = [](subtractive_synth_000& voice) { voice.key_off(); };
}

void sequencers::set_up_pad(composition& comp) {
    frsq_pad.voices = std::span<subtractive_synth_000>(comp.synth.pad.begin(), comp.synth.pad.end());
    frsq_pad.duration_absolute = comp.beat * 8.f;

    frsq_pad.duration_relative = 8.f;
    // frsq_pad.set_pattern(synth_patterns.patterns_pad[active_scene["pad"]]);

    // TODO: WHY DOES IT APPEAR THAT FRSQ SOMETIMES DOUBLE TRIGGERS OR DOUBLE ENDS?
    // is this an issue with println buffering, or is it actually double-eventing?
    frsq_pad.callback_start = [](subtractive_synth_000& voice, const event_midi& data) {
        // std::println("pad on {}", data.note);
        voice.key_on(data.note);
    };
    frsq_pad.callback_end = [](subtractive_synth_000& voice) {
        // std::println("pad off");
        voice.key_off();
    };
}

void sequencers::set_up_lead_a(composition& comp) {
    frsq_lead_a1.voices =
        std::span<subtractive_synth_000>(comp.synth.lead_a.begin(), comp.synth.lead_a.begin() + 3);
    frsq_lead_a1.duration_absolute = comp.beat * 4;
    frsq_lead_a1.duration_relative = 16.;
    // frsq_lead_a1.set_pattern(synth_patterns.patterns_lead_a[active_scene["lead_a1"]]);

    frsq_lead_a1.callback_start = [](subtractive_synth_000& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_lead_a1.callback_end = [](subtractive_synth_000& voice) { voice.key_off(); };

    frsq_lead_a2.voices =
        std::span<subtractive_synth_000>(comp.synth.lead_a.begin() + 3, comp.synth.lead_a.begin() + 6);
    frsq_lead_a2.duration_absolute = comp.beat * 8;
    frsq_lead_a2.duration_relative = 8.;
    // frsq_lead_a2.set_pattern(synth_patterns.patterns_lead_a[active_scene["lead_a2"]]);

    frsq_lead_a2.callback_start = [](subtractive_synth_000& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_lead_a2.callback_end = [](subtractive_synth_000& voice) { voice.key_off(); };
}

void sequencers::set_up_lead_b(composition& comp) {
    frsq_lead_b1.voices = std::span<nx_osc_000>(comp.synth.lead_b.begin(), comp.synth.lead_b.begin() + 3);
    frsq_lead_b1.duration_absolute = comp.beat * 4;
    frsq_lead_b1.duration_relative = 16.;
    // frsq_lead_b1.set_pattern(synth_patterns.patterns_lead_b[active_scene["lead_b1"]]);

    frsq_lead_b1.callback_start = [&](nx_osc_000& voice, const event_midi& data) {
        voice.note(data.note);
        voice.key_on();
        comp.synth.lead_b_lfo.note(data.note);
        comp.synth.lead_b_lfo.key_on();
    };
    frsq_lead_b1.callback_end = [](nx_osc_000& voice) { voice.key_off(); };

    frsq_lead_b2.voices = std::span<nx_osc_000>(comp.synth.lead_b.begin() + 3, comp.synth.lead_b.begin() + 6);
    frsq_lead_b2.duration_absolute = comp.beat * 4.;
    frsq_lead_b2.duration_relative = 8.;
    // frsq_lead_b2.set_pattern(synth_patterns.patterns_lead_b[active_scene["lead_b2"]]);

    frsq_lead_b2.callback_start = [](nx_osc_000& voice, const event_midi& data) {
        voice.note(data.note);
        voice.key_on();
    };
    frsq_lead_b2.callback_end = [](nx_osc_000& voice) { voice.key_off(); };
}

void sequencers::set_up_meta_sq(composition& comp) {
    meta_frsq_kick.voices    = std::span(&frsq_kick, 1);
    meta_frsq_bass.voices    = std::span(&frsq_bass, 1);
    meta_frsq_hh1.voices     = std::span(&frsq_hh1, 1);
    meta_frsq_hh2.voices     = std::span(&frsq_hh2, 1);
    meta_frsq_hh3.voices     = std::span(&frsq_hh3, 1);
    meta_frsq_ride.voices    = std::span(&frsq_ride, 1);
    meta_frsq_chord.voices   = std::span(&frsq_chord, 1);
    meta_frsq_lead_a1.voices = std::span(&frsq_lead_a1, 1);
    meta_frsq_lead_a2.voices = std::span(&frsq_lead_a2, 1);
    meta_frsq_lead_b1.voices = std::span(&frsq_lead_b1, 1);
    meta_frsq_lead_b2.voices = std::span(&frsq_lead_b2, 1);
    meta_frsq_pad.voices     = std::span(&frsq_pad, 1);

    meta_frsq_kick.duration_absolute    = comp.bar * 226;
    meta_frsq_bass.duration_absolute    = comp.bar * 226;
    meta_frsq_hh1.duration_absolute     = comp.bar * 226;
    meta_frsq_hh2.duration_absolute     = comp.bar * 226;
    meta_frsq_hh3.duration_absolute     = comp.bar * 226;
    meta_frsq_ride.duration_absolute    = comp.bar * 226;
    meta_frsq_chord.duration_absolute   = comp.bar * 226;
    meta_frsq_lead_a1.duration_absolute = comp.bar * 226;
    meta_frsq_lead_a2.duration_absolute = comp.bar * 226;
    meta_frsq_lead_b1.duration_absolute = comp.bar * 226;
    meta_frsq_lead_b2.duration_absolute = comp.bar * 226;
    meta_frsq_pad.duration_absolute     = comp.bar * 226;

    meta_frsq_kick.duration_relative    = 226;
    meta_frsq_bass.duration_relative    = 226;
    meta_frsq_hh1.duration_relative     = 226;
    meta_frsq_hh2.duration_relative     = 226;
    meta_frsq_hh3.duration_relative     = 226;
    meta_frsq_ride.duration_relative    = 226;
    meta_frsq_chord.duration_relative   = 226;
    meta_frsq_lead_a1.duration_relative = 226;
    meta_frsq_lead_a2.duration_relative = 226;
    meta_frsq_lead_b1.duration_relative = 226;
    meta_frsq_lead_b2.duration_relative = 226;
    meta_frsq_pad.duration_relative     = 226;

    meta_frsq_kick.callback_start = [&](frsq_019<kick_drum_000, event>& voice, const event_meta_sq& event) {
        std::println(" - frsq_kick:    {} @ {}", event.pattern_index, event.start_time);
        voice.set_pattern(this->drum_patterns.patterns_kick[event.pattern_index]);
    };
    meta_frsq_bass.callback_start = [&](frsq_019<subtractive_synth_000, event_midi>& voice,
                                        const event_meta_sq& event) {
        std::println(" - frsq_bass:    {} @ {}", event.pattern_index, event.start_time);
        voice.set_pattern(this->drum_patterns.patterns_bass[event.pattern_index]);
    };
    meta_frsq_hh1.callback_start = [&](frsq_019<cymbal_000, event>& voice, const event_meta_sq& event) {
        std::println(" - frsq_hh1:     {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->drum_patterns.patterns_hh1[event.pattern_index]);
    };
    meta_frsq_hh2.callback_start = [&](frsq_019<cymbal_000, event>& voice, const event_meta_sq& event) {
        std::println(" - frsq_hh2:     {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->drum_patterns.patterns_hh2[event.pattern_index]);
    };
    meta_frsq_hh3.callback_start = [&](frsq_019<subtractive_synth_000, event>& voice,
                                       const event_meta_sq& event) {
        std::println(" - frsq_hh3:     {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->drum_patterns.patterns_hh3[event.pattern_index]);
    };
    meta_frsq_ride.callback_start = [&](frsq_019<cymbal_000, event>& voice, const event_meta_sq& event) {
        std::println(" - frsq_ride:    {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->drum_patterns.patterns_ride[event.pattern_index]);
    };
    meta_frsq_chord.callback_start = [&](frsq_019<subtractive_synth_000, event_midi>& voice,
                                         const event_meta_sq& event) {
        std::println(" - frsq_chord:   {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->synth_patterns.patterns_chord[event.pattern_index]);
    };
    meta_frsq_lead_a1.callback_start = [&](frsq_019<subtractive_synth_000, event_midi>& voice,
                                           const event_meta_sq& event) {
        std::println(" - frsq_lead_a1: {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->synth_patterns.patterns_lead_a[event.pattern_index]);
    };
    meta_frsq_lead_a2.callback_start = [&](frsq_019<subtractive_synth_000, event_midi>& voice,
                                           const event_meta_sq& event) {
        std::println(" - frsq_lead_a2: {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->synth_patterns.patterns_lead_a[event.pattern_index]);
    };
    meta_frsq_lead_b1.callback_start = [&](frsq_019<nx_osc_000, event_midi>& voice,
                                           const event_meta_sq& event) {
        std::println(" - frsq_lead_b1: {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->synth_patterns.patterns_lead_b[event.pattern_index]);
    };
    meta_frsq_lead_b2.callback_start = [&](frsq_019<nx_osc_000, event_midi>& voice,
                                           const event_meta_sq& event) {
        std::println(" - frsq_lead_b2: {} @ {}", event.pattern_index, event.start_time);

        voice.set_pattern(this->synth_patterns.patterns_lead_b[event.pattern_index]);
    };
    meta_frsq_pad.callback_start = [&](frsq_019<subtractive_synth_000, event_midi>& voice,
                                       const event_meta_sq& event) {
        std::println(" - frsq_pad:     {} @ {}", event.pattern_index, event.start_time);
        voice.set_pattern(this->synth_patterns.patterns_pad[event.pattern_index]);
    };

    meta_frsq_kick.set_pattern(meta_patterns.patterns_kick, meta_patterns.PLAYBACK_START);
    meta_frsq_bass.set_pattern(meta_patterns.patterns_bass, meta_patterns.PLAYBACK_START);
    meta_frsq_hh1.set_pattern(meta_patterns.patterns_hh1, meta_patterns.PLAYBACK_START);
    meta_frsq_hh2.set_pattern(meta_patterns.patterns_hh2, meta_patterns.PLAYBACK_START);
    meta_frsq_hh3.set_pattern(meta_patterns.patterns_hh3, meta_patterns.PLAYBACK_START);
    meta_frsq_ride.set_pattern(meta_patterns.patterns_ride, meta_patterns.PLAYBACK_START);
    meta_frsq_chord.set_pattern(meta_patterns.patterns_chord, meta_patterns.PLAYBACK_START);
    meta_frsq_lead_a1.set_pattern(meta_patterns.patterns_lead_a1, meta_patterns.PLAYBACK_START);
    meta_frsq_lead_a2.set_pattern(meta_patterns.patterns_lead_a2, meta_patterns.PLAYBACK_START);
    meta_frsq_lead_b1.set_pattern(meta_patterns.patterns_lead_b1, meta_patterns.PLAYBACK_START);
    meta_frsq_lead_b2.set_pattern(meta_patterns.patterns_lead_b2, meta_patterns.PLAYBACK_START);
    meta_frsq_pad.set_pattern(meta_patterns.patterns_pad, meta_patterns.PLAYBACK_START);
}

void sequencers::set_up_arrangement_print(composition& comp) {
    frsq_arrangement_print.voices            = std::span(meta_patterns.timing_range.begin(), 1);
    frsq_arrangement_print.duration_absolute = comp.bar * 224;
    frsq_arrangement_print.duration_relative = 224;
    frsq_arrangement_print.set_pattern(meta_patterns.timing_range);
    frsq_arrangement_print.callback_start = [](event& voice, const event& event) {
        // clang-format off
        if (
            event.start_time == 0   ||
            event.start_time == 32  ||
            event.start_time == 65  ||
            event.start_time == 89  ||
            event.start_time == 97  ||
            event.start_time == 125 ||
            event.start_time == 137 ||
            event.start_time == 145 ||
            event.start_time == 161 ||
            event.start_time == 177 ||
            event.start_time == 185 ||
            event.start_time == 193 ||
            event.start_time == 209 ||
            event.start_time == 225
            ){
            // clang-format on
            std::println("--------------------------------");
        } else {
            std::println("--------");
        }
        std::println("B: {}", event.start_time);
    };
}
