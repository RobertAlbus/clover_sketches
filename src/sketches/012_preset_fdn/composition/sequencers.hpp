#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "kick.hpp"
#include "patterns.hpp"
#include "stsq.hpp"

struct sequencers {
    stsq<int> kick_sequence;
    stsq<int> clap_sequence;
    stsq<int> hhat_sequence;

    sequencers(double fs, double bpm, kick_drum& kick) {
        double spm = fs * 60;
        double spb = spm / bpm;
        double bar = spb * 4;

        kick_sequence.pattern_duration_samples = bar;

        kick_sequence.num_steps = 16;

        kick_sequence.step_duration_samples = bar / 16;

        kick_sequence.pattern = &pattern::kick_map;

        kick_sequence.callback = [&](const int clock, const int step, const int& step_data) {
            switch (step_data) {
                case pattern::X:
                case pattern::x:
                    kick.key_on();
                    break;
                case pattern::_:
                    kick.key_off();
                    break;
            }
        };
    }

    void tick() {
        kick_sequence.tick();
    }
};