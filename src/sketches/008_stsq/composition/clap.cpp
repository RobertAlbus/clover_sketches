// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <map>
#include <ranges>

#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"

#include "clap.hpp"

using namespace clover;
using namespace dsp;

clap_envelope::clap_envelope(int pulse_count, int spacing, int duration, int duration_final) {
    count = ((pulse_count - 1) * spacing) + duration_final;
    for (auto i : std::views::iota(0, pulse_count))
        triggers.insert({i * spacing, float(i == pulse_count - 1 ? duration_final : duration)});
}

clover_float clap_envelope::tick() {
    if (triggers.contains(count)) {
        env.set(1, 0, triggers.at(count));
    }
    ++count;

    return env.tick();
}

void clap_envelope::trigger() {
    count = 0;
}

hand_clap::hand_clap() : noise_generator(fs), clap_env(5, 48 * 15, 48 * 5, 48 * 50) {
    noise_generator.waveform = wave_noise;
    eqs[0].m_coeffs          = eq(fs, 400, 0.5, 10);
    eqs[1].m_coeffs          = eq(fs, 2500, 1.4, 2);
    eqs[2].m_coeffs          = lpf(fs, 4000, 1.4);
};

std::pair<clover_float, clover_float> hand_clap::tick() {
    float clap_env_signal = clap_env.tick();

    float mid     = noise_generator.tick();
    float side    = noise_generator.tick() * 0.8f;
    float noise_L = mid + side;
    float noise_R = mid - side;

    for (auto &eq : eqs) {
        auto [eq_L, eq_R] = eq.tick(noise_L, noise_R);
        noise_L           = eq_L;
        noise_R           = eq_R;
    }

    if (counter == snare_key_on) {
        clap_env.trigger();
    }

    if (++counter == 48000) {
        counter = 0;
    }

    return {(noise_L * clap_env_signal), (noise_R * clap_env_signal)};
};
