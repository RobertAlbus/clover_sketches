#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_automation_clip.hpp"

using namespace clover;
using namespace dsp;

std::map<int64_t, float> automation_points_verb_in(float fs, float bpm);
std::map<int64_t, float> automation_points_dry(float fs, float bpm);

struct verb_automation {
    float verb_in  = 0;
    float verb_dry = 0;
    env_automation_clip automation_verb_in;
    env_automation_clip verb_automation_dry;

    verb_automation(float fs, float bpm);

    void tick();
};
