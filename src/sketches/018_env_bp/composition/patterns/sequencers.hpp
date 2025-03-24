#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/cymbal.hpp"
#include "composition/instruments/env_bp.hpp"
#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/frsq.hpp"
#include "composition/instruments/kick.hpp"
#include "composition/instruments/nx_osc.hpp"
#include "composition/instruments/stsq.hpp"
#include "composition/instruments/subtractive_synth.hpp"

#include "patterns.hpp"
#include <ranges>
#include <span>

struct sequencers {
    stsq<int> kick_sequencer;
    stsq<int> hh1_stsq;
    stsq<std::array<float, 4>> chord_sequencer;

    using subtractive_sq_t = frsq<pattern::midi_event, subtractive_synth>;
    subtractive_sq_t subtractive_synth_sequencer;
    frsq<pattern::meta_pattern, subtractive_sq_t> subtractive_synth_metasq;

    frsq<pattern::midi_event, cymbal> hh2_frsq;
    frsq<pattern::midi_event, env_bp> hh2_articulation_frsq;

    sequencers(
            double fs,
            double bpm,
            kick_drum& kick,
            cymbal& hh1,
            cymbal& hh2,
            env_bp& hh2_articulation,
            std::array<nx_osc, 4>& chords,
            std::array<filter_block, 4>& chord_filters,
            std::array<subtractive_synth, 4>& drones) {
        double spm = fs * 60;
        double spb = spm / bpm;
        double bar = spb * 4;

        kick_sequencer.pattern_duration_samples = bar;
        kick_sequencer.step_duration_samples    = bar / 16;

        kick_sequencer.num_steps = 16;
        kick_sequencer.pattern   = &pattern::kick_map;
        kick_sequencer.callback  = [&](const int clock, const int step, const int& step_data) {
            switch (step_data) {
                case pattern::X:
                case pattern::x:
                    kick.key_on();
                    break;
                case pattern::_:
                    kick.key_off();
                    break;
                default:
                    break;
            }
        };

        hh1_stsq.pattern_duration_samples = bar;
        hh1_stsq.step_duration_samples    = bar / 16;

        hh1_stsq.num_steps = 16;
        hh1_stsq.pattern   = &pattern::hh_map;
        hh1_stsq.callback  = [&](const int clock, const int step, const int& step_data) {
            switch (step_data) {
                case pattern::X:
                case pattern::x:
                    hh1.key_on();
                    break;
                case pattern::_:
                    hh1.key_off();
                    break;
                default:
                    break;
            }
        };

        hh2_frsq.callback_start = [](cymbal& voice, const pattern::midi_event& data) { voice.key_on(); };
        hh2_frsq.callback_end   = [](cymbal& voice) { voice.key_off(); };
        hh2_frsq.voices         = std::span<cymbal>(&hh2, 1);
        hh2_frsq.set_pattern(pattern::hh2_pattern);
        hh2_frsq.duration_absolute = bar / 4;
        hh2_frsq.duration_relative = 4;

        hh2_articulation_frsq.callback_start = [](env_bp& voice, const pattern::midi_event& data) {
            voice.key_on();
        };
        hh2_articulation_frsq.voices = std::span<env_bp>(&hh2_articulation, 1);
        hh2_articulation_frsq.set_pattern(pattern::hh2_articulation_trigger_pattern);
        hh2_articulation_frsq.duration_absolute = (bar / 16) * 5.;
        hh2_articulation_frsq.duration_relative = 5;

        chord_sequencer.pattern_duration_samples = bar;
        chord_sequencer.num_steps                = 16;
        chord_sequencer.step_duration_samples    = bar / 16;
        chord_sequencer.pattern                  = &pattern::chord_map;
        chord_sequencer.callback =
                [&](const int clock, const int step, const std::array<float, 4>& step_data) {
                    for (auto [note, voice, filter] : std::views::zip(step_data, chords, chord_filters)) {
                        if (note < 0) {
                            voice.key_off();
                            filter.key_off();
                        } else {
                            voice.note(note);
                            voice.key_on();
                            filter.key_on();
                        }
                    }
                };

        subtractive_synth_metasq.callback_start = [](subtractive_sq_t& sq,
                                                     const pattern::meta_pattern& data) {
            sq.set_pattern(pattern::beep_patterns[data.pattern_index]);
        };

        subtractive_synth_metasq.voices = std::span<subtractive_sq_t>(&subtractive_synth_sequencer, 1);
        subtractive_synth_metasq.set_pattern(pattern::beep_meta_pattern);

        subtractive_synth_metasq.duration_absolute = 8 * bar;
        subtractive_synth_metasq.duration_relative = 8;

        subtractive_synth_sequencer.callback_start =
                [](subtractive_synth& voice, const pattern::midi_event& data) { voice.key_on(data.note); };
        subtractive_synth_sequencer.callback_end = [](subtractive_synth& voice) { voice.key_off(); };

        subtractive_synth_sequencer.voices = drones;

        subtractive_synth_sequencer.duration_absolute = bar;
        subtractive_synth_sequencer.duration_relative = 16;
    }

    void tick() {
        kick_sequencer.tick();
        hh1_stsq.tick();
        hh2_frsq.tick();
        hh2_articulation_frsq.tick();
        chord_sequencer.tick();
        subtractive_synth_metasq.tick();
        subtractive_synth_sequencer.tick();
    }
};