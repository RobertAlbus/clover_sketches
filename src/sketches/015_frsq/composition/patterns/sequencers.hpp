#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/cymbal.hpp"
#include "composition/instruments/drone.hpp"
#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/frsq.h"
#include "composition/instruments/kick.hpp"
#include "composition/instruments/nx_osc.hpp"
#include "composition/instruments/stsq.hpp"

#include "patterns.hpp"
#include <ranges>

struct sequencers {
    stsq<int> kick_sequencer;
    stsq<int> hh_sequencer;
    stsq<std::array<float, 4>> chord_sequencer;
    frsq<pattern::midi_event, drone_synth> drone_sequencer;

    sequencers(
            double fs,
            double bpm,
            kick_drum& kick,
            cymbal& hh,
            std::array<nx_osc, 4>& chords,
            std::array<filter_block, 4>& chord_filters,
            std::array<drone_synth, 4>& drones) {
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

        hh_sequencer.pattern_duration_samples = bar;
        hh_sequencer.step_duration_samples    = bar / 16;

        hh_sequencer.num_steps = 16;
        hh_sequencer.pattern   = &pattern::hh_map;
        hh_sequencer.callback  = [&](const int clock, const int step, const int& step_data) {
            switch (step_data) {
                case pattern::X:
                case pattern::x:
                    hh.key_on();
                    break;
                case pattern::_:
                    hh.key_off();
                    break;
                default:
                    break;
            }
        };

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
        drone_sequencer.callback_start = [](drone_synth& voice, const pattern::midi_event& data) {
            voice.key_on(data.note);
        };
        drone_sequencer.callback_end = [](drone_synth& voice) { voice.key_off(); };

        drone_sequencer.voices = drones;
        drone_sequencer.set_pattern(pattern::drone_pattern);

        drone_sequencer.duration_absolute     = bar;
        drone_sequencer.duration_relative     = 16;
        drone_sequencer.current_time_absolute = 0;
    }

    void tick() {
        kick_sequencer.tick();
        hh_sequencer.tick();
        chord_sequencer.tick();
        drone_sequencer.tick();
    }
};