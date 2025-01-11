// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clap_env.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/math.hpp"
#include "util/fdl.hpp"

#include "composition_kick.hpp"

using namespace clover;
using namespace dsp;
using namespace io;

struct composition {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator noise_generator;

    filter_2 eqs[3];

    s03_fdl_lagrange_2 delay{96000};

    int_fast64_t counter       = 0;
    int_fast64_t snare_key_on  = 24500;
    int_fast64_t snare_key_off = 33000;

    kick_drum kick;
    clap_envelope clap_env;

    float mix_clap = db_to_linear(-6);

    composition() : noise_generator(fs), clap_env(5, 48 * 15, 48 * 5, 48 * 50) {
        noise_generator.waveform = wave_noise;
        eqs[0].m_coeffs          = eq(fs, 400, 0.5, 10);
        eqs[1].m_coeffs          = eq(fs, 2500, 1.4, 2);
        eqs[2].m_coeffs          = lpf(fs, 4000, 1.4);
    };

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        float clap_env_signal = clap_env.tick();
        float kick_signal     = kick.tick(counter);

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

        L = kick_signal + (noise_L * clap_env_signal * mix_clap);
        R = kick_signal + (noise_R * clap_env_signal * mix_clap);

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
