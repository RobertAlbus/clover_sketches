#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clap.hpp"
#include "cymbal.hpp"
#include "kick.hpp"
#include "patterns.hpp"
#include "stsq.hpp"

struct sequencers {
    stsq<int> kick_sequence;
    stsq<int> clap_sequence;
    stsq<int> hhat_sequence;

    sequencers(double fs, double bpm, kick_drum& kick, hand_clap& clap, cymbal& hhat) {
        double spm = fs * 60;
        double spb = spm / bpm;
        double bar = spb * 4;

        kick_sequence.pattern_duration_samples = bar;
        clap_sequence.pattern_duration_samples = 2 * bar;
        hhat_sequence.pattern_duration_samples = bar;

        kick_sequence.num_steps = 16;
        clap_sequence.num_steps = 32;
        hhat_sequence.num_steps = 16;

        kick_sequence.step_duration_samples = bar / 16;
        clap_sequence.step_duration_samples = bar / 16;
        hhat_sequence.step_duration_samples = bar / 16;

        kick_sequence.pattern = &pattern::kick_map;
        clap_sequence.pattern = &pattern::clap_map;
        hhat_sequence.pattern = &pattern::hhat_map;

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
        clap_sequence.callback = [&](const int clock, const int step, const int& step_data) {
            switch (step_data) {
                case pattern::X:
                    clap.key_on();
                    clap.gain = 1;
                    break;
                case pattern::x:
                    clap.key_on();
                    clap.gain = 0.6;
                    break;
                case pattern::_:
                    clap.key_off();
                    break;
            }
        };
        hhat_sequence.callback = [&](const int clock, const int step, const int& step_data) {
            switch (step_data) {
                case pattern::X:
                case pattern::x:
                    hhat.key_on();
                    break;
                case pattern::_:
                    hhat.key_off();
                    break;
            }
            if (step_data == pattern::X || step_data == pattern::x) {
                hhat.key_on();
            } else if (step_data == pattern::_) {
                hhat.key_off();
            }
        };
    }

    void tick() {
        kick_sequence.tick();
        clap_sequence.tick();
        hhat_sequence.tick();
    }
};