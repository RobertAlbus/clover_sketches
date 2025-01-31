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
    automation.tick();

    float signal_kick = kick.tick();
    signal_kick *= 1.5f;
    signal_kick = std::clamp(signal_kick, -0.98f, 0.98f);
    signal_kick *= 0.6f;

    float signal_hat = hhat.tick();
    signal_hat *= 0.6f;

    auto [clap_L, clap_R] = clap.tick();
    clap_L *= 0.2;
    clap_R *= 0.2;

    float loop_L = signal_kick + signal_hat + clap_L;
    float loop_R = signal_kick + signal_hat + clap_R;

    float loop_output_L = loop_L * loop_mix;  // * automation.verb_dry;
    float loop_output_R = loop_R * loop_mix;  // * automation.verb_dry;

    float verb_output_L = reverb_L.tick(loop_L * verb_in_gain) * reverb_mix;  // * automation.verb_in;
    float verb_output_R = reverb_R.tick(loop_R * verb_in_gain) * reverb_mix;  // * automation.verb_in;

    float out_L = loop_output_L + verb_output_L;
    float out_R = loop_output_R + verb_output_R;

    return {out_L, out_R};
}