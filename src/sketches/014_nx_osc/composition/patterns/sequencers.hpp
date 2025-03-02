#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/kick.hpp"
#include "composition/instruments/nx_osc.hpp"
#include "composition/instruments/stsq.hpp"

#include "patterns.hpp"
#include <ranges>

struct sequencers {
    stsq<int> kick_sequencer;
    stsq<std::array<float, 4>> chord_sequencer;

    sequencers(double fs, double bpm, kick_drum& kick, std::array<nx_osc, 4>& chords) {
        double spm = fs * 60;
        double spb = spm / bpm;
        double bar = spb * 4;

        kick_sequencer.pattern_duration_samples = bar;
        kick_sequencer.num_steps                = 16;
        kick_sequencer.step_duration_samples    = bar / 16;
        kick_sequencer.pattern                  = &pattern::kick_map;
        kick_sequencer.callback                 = [&](const int clock, const int step, const int& step_data) {
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

        chord_sequencer.pattern_duration_samples = bar;
        chord_sequencer.num_steps                = 16;
        chord_sequencer.step_duration_samples    = bar / 16;
        chord_sequencer.pattern                  = &pattern::chord_map;
        chord_sequencer.callback =
                [&](const int clock, const int step, const std::array<float, 4>& step_data) {
                    for (auto [note, chord_note] : std::views::zip(step_data, chords)) {
                        if (note < 0) {
                            chord_note.key_off();
                            // std::println("chord key_off");
                        } else {
                            // std::println("chord key_on {}", note);
                            chord_note.note(note);
                            chord_note.key_on();
                        }
                    }
                };
    }

    void tick() {
        kick_sequencer.tick();
        chord_sequencer.tick();
    }
};