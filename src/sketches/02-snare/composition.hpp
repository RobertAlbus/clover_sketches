// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <ranges>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/float.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/math.hpp"
#include "util/math.hpp"

using namespace clover;
using namespace dsp;
using namespace io;

struct composition {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 4 * 60 * fs_i;
    int channel_count_out = 2;

    oscillator harmonic_osc[3]{{fs}, {fs}, {fs}};
    clover_float harmonic_fundamental = 112;
    clover_float harmonic_freqs[3]{
            harmonic_fundamental, harmonic_fundamental * 1.33f, harmonic_fundamental * 1.91f};
    clover_float harmonic_gain[3]{0.7, 1, 0.6};
    clover_float harmonics_pitch_range_octaves = 1.5;
    env_adsr harmonic_pitch_adsr;
    env_adsr harmonic_gain_adsr;

    oscillator noise{fs};
    env_adsr noise_gain_adsr;

    filter noise_filter;
    clover_float noise_filter_f0 = 880;
    clover_float noise_filter_f1 = 4000;
    clover_float noise_filter_octaves =
            sketch::octave_difference_by_frequency(noise_filter_f0, noise_filter_f1);

    env_adsr noise_cutoff_adsr;
    clover_float noise_filter_Q1      = 1.5;
    clover_float noise_filter_Q2      = 2;
    clover_float noise_filter_Q_delta = noise_filter_Q2 - noise_filter_Q1;
    env_adsr noise_Q_adsr;

    filter equalizer[2]{};
    ;

    clover_float harmonic_mix = db_to_linear(-12);
    clover_float noise_mix    = db_to_linear(-6);
    clover_float gain         = db_to_linear(-6);

    int_fast64_t counter = 0;

    composition() {
        noise.waveform        = wave_noise;
        noise_filter.m_coeffs = bpf(fs, noise_filter_f0, noise_filter_Q1);
        equalizer[0].m_coeffs = eq(fs, 120, 3, db_to_linear(2));
        equalizer[1].m_coeffs = eq(fs, 12000, 3, db_to_linear(6));

        harmonic_pitch_adsr.set(1, 1000, 0, 1000);
        harmonic_gain_adsr.set(1, 3000, 0, 1000);
        noise_cutoff_adsr.set(1, 1000, 0, 1000);
        noise_Q_adsr.set(1, 1000, 0, 1000);
        noise_gain_adsr.set(1, 2000, 0, 1000);
    }

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        if (counter == 24000) {
            for (auto i : std::views::iota(0, 3))
                harmonic_osc[i].phase(0);

            harmonic_pitch_adsr.key_on();
            harmonic_gain_adsr.key_on();
            noise_cutoff_adsr.key_on();
            noise_Q_adsr.key_on();
            noise_gain_adsr.key_on();

        } else if (counter == 29000) {
            harmonic_pitch_adsr.key_off();
            harmonic_gain_adsr.key_off();
            noise_cutoff_adsr.key_off();
            noise_Q_adsr.key_off();
            noise_gain_adsr.key_off();
        }

        if (++counter == 48000) {
            counter = 0;
        }

        clover_float pitch_adsr = harmonic_pitch_adsr.tick();
        clover_float gain_adsr  = harmonic_gain_adsr.tick();

        clover_float harmonics_signal = 0;
        for (auto i : std::views::iota(0, 3)) {
            harmonic_osc[i].freq(frequency_by_octave_difference(
                    harmonic_freqs[i], harmonics_pitch_range_octaves * pitch_adsr));
            harmonics_signal = harmonic_osc[i].tick() * harmonic_gain[i] * gain_adsr;
        }

        clover_float noise_signal = noise.tick();
        clover_float noise_gain   = noise_gain_adsr.tick();

        clover_float noise_cutoff = frequency_by_octave_difference(
                noise_filter_f0, noise_filter_octaves * noise_cutoff_adsr.tick());
        clover_float noise_Q  = noise_filter_Q1 + (noise_filter_Q_delta * noise_Q_adsr.tick());
        noise_filter.m_coeffs = bpf(fs, noise_cutoff, noise_Q);

        clover_float noise_filtered = noise_filter.tick(noise_signal * noise_gain);

        clover_float pre_eq = gain * ((noise_filtered * noise_mix) + (harmonics_signal * harmonic_mix));

        L = equalizer[0].tick(equalizer[1].tick(pre_eq));
        R = L;

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
