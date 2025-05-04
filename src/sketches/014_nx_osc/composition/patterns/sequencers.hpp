#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/kick.hpp"
#include "composition/instruments/nx_osc.hpp"
#include "lib/stsq/stsq.hpp"

#include "patterns.hpp"
#include <ranges>

struct sequencers {
    stsq_000<int> kick_sequencer;
    stsq_000<std::array<float, 4>> chord_sequencer;

    sequencers(
            double fs,
            double bpm,
            kick_drum& kick,
            std::array<nx_osc, 4>& chords,
            std::array<filter_block, 4>& chord_filters) {
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
    }

    void tick() {
        kick_sequencer.tick();
        chord_sequencer.tick();
    }
};