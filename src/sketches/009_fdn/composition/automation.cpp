// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "automation.hpp"

using namespace clover;
using namespace dsp;

std::map<int64_t, float> automation_points_verb_in(float fs, float bpm) {
    double spm = fs * 60;
    double spb = spm / bpm;
    double bar = spb * 4;

    return {
            {0, 0},               //
            {2 * bar, 0},         //
            {4 * bar, 1},         //
            {(12 * bar) - 1, 1},  //
            {(12 * bar), 0},      //
            {(14 * bar) - 1, 0},  //
            {14 * bar, 1},        //
    };
}
std::map<int64_t, float> automation_points_dry(float fs, float bpm) {
    double spm = fs * 60;
    double spb = spm / bpm;
    double bar = spb * 4;

    return {
            {0, 1},              //
            {(6 * bar) - 1, 1},  //
            {(6 * bar), 0},      //
            {(8 * bar) - 1, 0},  //
            {(8 * bar), 1},      //
    };
}

verb_automation::verb_automation(float fs, float bpm)
    : automation_verb_in{automation_points_verb_in(fs, bpm)},
      verb_automation_dry{automation_points_dry(fs, bpm)} {
}

void verb_automation::tick() {
    verb_in  = automation_verb_in.tick();
    verb_dry = verb_automation_dry.tick();
}
