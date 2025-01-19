// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>

#include "composition.hpp"

using namespace clover;
using namespace io;

std::pair<clover_float, clover_float> composition::tick() {
    stsqs.tick();

    float signal_kick = kick.tick();
    signal_kick *= 1.5f;
    signal_kick = std::clamp(signal_kick, -0.98f, 0.98f);
    signal_kick *= 0.6f;

    float signal_hat = hhat.tick();
    signal_hat *= 0.6f;

    auto [clap_L, clap_R] = clap.tick();
    clap_L *= 0.2;
    clap_R *= 0.2;

    return {signal_kick + signal_hat + clap_L, signal_kick + signal_hat + clap_R};
}