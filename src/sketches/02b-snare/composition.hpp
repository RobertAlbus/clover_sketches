// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/float.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/math.hpp"

#include "composition_snare_body.hpp"
#include "util/math.hpp"

using namespace clover;
using namespace dsp;
using namespace io;

struct composition {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator noise{fs};
    env_adsr noise_gain_adsr;

    filter_2 noise_filter;
    env_adsr noise_cutoff_adsr;
    clover_float noise_filter_f0 = 1880;
    clover_float noise_filter_f1 = 2400;
    clover_float noise_filter_octaves =
            sketch::octave_difference_by_frequency(noise_filter_f0, noise_filter_f1);
    clover_float noise_filter_Q1      = 1;
    clover_float noise_filter_Q2      = 1.5;
    clover_float noise_filter_Q_delta = noise_filter_Q2 - noise_filter_Q1;
    env_adsr noise_Q_adsr;
    iir_coeffs (*filter_func)(clover_float, clover_float, clover_float) = &lpf;

    filter_2 equalizer[2]{};

    clover_float body_mix  = db_to_linear(-12);
    clover_float noise_mix = db_to_linear(-6);
    clover_float gain      = db_to_linear(-3);

    int_fast64_t counter       = 0;
    int_fast64_t snare_key_on  = 24500;
    int_fast64_t snare_key_off = 29000;

    snare_body::ctor_args snare_body_args = {
            .pitch_fundamental  = 150,
            .pitch_peak         = 400,
            .cutoff_fundamental = 150,
            .cutoff_peak        = 400,
            .adsr_mult          = 0.8,
            .counter_on         = snare_key_on,
            .counter_off        = snare_key_off,
    };
    snare_body body{snare_body_args};

    composition() {
        noise.waveform        = wave_noise;
        noise_filter.m_coeffs = bpf(fs, noise_filter_f0, noise_filter_Q1);
        equalizer[0].m_coeffs = eq(fs, 420, 1, db_to_linear(2));
        equalizer[1].m_coeffs = eq(fs, 3000, 1, db_to_linear(1));

        clover_float adsr_mult = 0.8;
        noise_cutoff_adsr.set(1, 1000 * adsr_mult, 0, 1000);
        noise_Q_adsr.set(1, 1000 * adsr_mult, 0, 1000);
        noise_gain_adsr.set(50, 1600 * adsr_mult, 0, 1000);
    }

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        if (counter == snare_key_on + 30) {
            noise_cutoff_adsr.key_on();
            noise_Q_adsr.key_on();
            noise_gain_adsr.key_on();

        } else if (counter == snare_key_off) {
            noise_cutoff_adsr.key_off();
            noise_Q_adsr.key_off();
            noise_gain_adsr.key_off();
        }

        if (++counter == 48000) {
            counter = 0;
        }

        clover_float noise_gain     = noise_gain_adsr.tick();
        clover_float noise_signal_L = noise.tick() * noise_gain * noise_mix;
        clover_float noise_signal_R = noise.tick() * noise_gain * noise_mix;

        clover_float noise_cutoff = frequency_by_octave_difference(
                noise_filter_f0, noise_filter_octaves * noise_cutoff_adsr.tick());

        clover_float noise_Q  = noise_filter_Q1 + (noise_filter_Q_delta * noise_Q_adsr.tick());
        noise_filter.m_coeffs = filter_func(fs, noise_cutoff, noise_Q);

        auto [noise_filtered_L, noise_filtered_R] = noise_filter.tick(noise_signal_L, noise_signal_R);

        clover_float snare_body = body.tick(counter) * body_mix;

        clover_float pre_eq_L = gain * (noise_filtered_L + snare_body);
        clover_float pre_eq_R = gain * (noise_filtered_R + snare_body);

        auto [eq_L, eq_R]    = equalizer[0].tick(pre_eq_L, pre_eq_R);
        std::tie(eq_L, eq_R) = equalizer[1].tick(eq_L, eq_R);

        L = eq_L;
        R = eq_R;

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
