#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/clap.hpp"
#include "composition/cymbal.hpp"
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
        clap_sequence.pattern_duration_samples = bar;
        hhat_sequence.pattern_duration_samples = bar;

        kick_sequence.pattern = &pattern::kick_map;
        clap_sequence.pattern = &pattern::hhat_map;
        hhat_sequence.pattern = &pattern::clap_map;

        kick_sequence.callback = [this](const int& step_data) {

        };
        clap_sequence.callback = [this](const int& step_data) {

        };
        hhat_sequence.callback = [this](const int& step_data) {

        };
    }

    void tick() {
        kick_sequence.tick();
        clap_sequence.tick();
        hhat_sequence.tick();
    }
};