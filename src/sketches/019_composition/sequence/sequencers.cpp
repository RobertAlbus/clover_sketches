// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <map>
#include <print>

#include "composition/composition.hpp"
#include "instruments/kick.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequencers.hpp"

std::map<const char*, size_t> scene_1 = {
        {"kick", 1},
        {"bass", 1},
        {"hh1", 1},
        {"hh2", 1},
};

std::map<const char*, size_t>& active_scene = scene_1;

sequencers::sequencers(composition& comp) {
    set_up_kick(comp);
    set_up_bass(comp);
    set_up_hh1(comp);
    set_up_hh2(comp);
}

void sequencers::tick() {
    frsq_kick.tick();
    frsq_bass.tick();
    frsq_hh1.tick();
    frsq_hh2.tick();
}

void sequencers::set_up_kick(composition& comp) {
    frsq_kick.voices            = std::span<kick_drum>(&comp.kick.drum, 1);
    frsq_kick.duration_absolute = comp.beat;
    frsq_kick.duration_relative = 1.;
    frsq_kick.set_pattern(drum_patterns.patterns_kick[active_scene["kick"]]);

    frsq_kick.callback_start = [](kick_drum& voice, const event& data) { voice.key_on(); };
    frsq_kick.callback_end   = [](kick_drum& voice) { voice.key_off(); };
}
void sequencers::set_up_bass(composition& comp) {
    frsq_bass.voices            = std::span<subtractive_synth>(&comp.bass.synth, 1);
    frsq_bass.duration_absolute = comp.beat * 4;
    frsq_bass.duration_relative = 8.;
    frsq_bass.set_pattern(drum_patterns.patterns_bass[active_scene["bass"]]);

    frsq_bass.callback_start = [](subtractive_synth& voice, const event_midi& data) {
        voice.key_on(data.note);
    };
    frsq_bass.callback_end = [](subtractive_synth& voice) { voice.key_off(); };
}

void sequencers::set_up_hh1(composition& comp) {
    frsq_hh1.voices            = std::span<cymbal>(&comp.cymbals.hh1, 1);
    frsq_hh1.duration_absolute = comp.beat * 2;
    frsq_hh1.duration_relative = 2.;
    frsq_hh1.set_pattern(drum_patterns.patterns_hh1[active_scene["hh1"]]);

    frsq_hh1.callback_start = [](cymbal& voice, const event& data) { voice.key_on(); };
    frsq_hh1.callback_end   = [](cymbal& voice) { voice.key_off(); };
}

void sequencers::set_up_hh2(composition& comp) {
    frsq_hh2.voices            = std::span<cymbal>(&comp.cymbals.hh2, 1);
    frsq_hh2.duration_absolute = comp.beat * 2;
    frsq_hh2.duration_relative = 2.;
    frsq_hh2.set_pattern(drum_patterns.patterns_hh2[active_scene["hh2"]]);

    frsq_hh2.callback_start = [](cymbal& voice, const event& data) { voice.key_on(); };
    frsq_hh2.callback_end   = [](cymbal& voice) { voice.key_off(); };
}
