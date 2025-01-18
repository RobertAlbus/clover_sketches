// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <map>
#include <ranges>

#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/fractional_delay.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"

#include "composition_kick.hpp"

using namespace clover;
using namespace dsp;

struct trigger_info {
    int start_time;
    float env_duration;
};
struct clap_envelope {
    clover::dsp::env_linear env;
    int duration;
    int count;

    std::map<int, float> triggers;

    clap_envelope(int pulse_count, int spacing, int duration, int duration_final) {
        count = ((pulse_count - 1) * spacing) + duration_final;
        for (auto i : std::views::iota(0, pulse_count))
            triggers.insert({i * spacing, float(i == pulse_count - 1 ? duration_final : duration)});
    }

    clover_float tick() {
        if (triggers.contains(count)) {
            env.set(1, 0, triggers.at(count));
        }
        ++count;

        return env.tick();
    }

    void trigger() {
        count = 0;
    }
};

struct hand_clap {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator noise_generator;

    filter_2 eqs[3];

    fdl_lagrange_2 delay{96000};

    int_fast64_t counter       = 0;
    int_fast64_t snare_key_on  = 24000;
    int_fast64_t snare_key_off = 33000;

    kick_drum kick;
    clap_envelope clap_env;

    float mix_clap = db_to_linear(-6);

    hand_clap() : noise_generator(fs), clap_env(5, 48 * 15, 48 * 5, 48 * 50) {
        noise_generator.waveform = wave_noise;
        eqs[0].m_coeffs          = eq(fs, 400, 0.5, 10);
        eqs[1].m_coeffs          = eq(fs, 2500, 1.4, 2);
        eqs[2].m_coeffs          = lpf(fs, 4000, 1.4);
    };

    std::pair<clover_float, clover_float> tick() {
        float clap_env_signal = clap_env.tick();
        float kick_signal     = kick.tick();

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

        return {kick_signal + (noise_L * clap_env_signal * mix_clap),
                kick_signal + (noise_R * clap_env_signal * mix_clap)};
    };
};
