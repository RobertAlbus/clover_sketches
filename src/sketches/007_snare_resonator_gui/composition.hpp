#pragma once
// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <ranges>

#include "clover/dsp/env_linear.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/io/audio_callback.hpp"
#include "clover/math.hpp"

#include "resonator_section.hpp"
#include "smooth.hpp"

using namespace clover;
using namespace dsp;
using namespace io;

/*
transient strike
    - white noise burst - add randomization to this to "humanize"
    - square wave for body
- resonators (delay and filtering) represent the drum head and runing
    - lp/hp or bp, plus a dip with eq to tune the overtones
- pitch of the square in the transient is the resonance of the body
- white noise with bump at 2k to simulate snare wires
    - lower pitch drum would have longer duration snare vibration
- mix to taste
- add "the room" aka reverb
- a bit of distortion over top of all things
*/

/*
| Mode               | freq ratio | decay ratio   | decay (ms) | rel amp (dB)   |
|--------------------|------------|---------------|------------|----------------|
| 0th          (0,1) | 1.00       | 1.00          | 300–800    |  0              |
| 1st Overtone (1,1) | 1.59       | 0.67 – 0.83   | 200–500    | -3 to -10      |
| 2nd Overtone (2,1) | 2.14       | 0.50 – 0.67   | 150–400    | -6 to -15      |
| 3rd Overtone (0,2) | 2.30       | 0.40 – 0.58   | 120–350    | -10 to -20     |
| 4th Overtone (3,1) | 2.65       | 0.33 – 0.50   | 100–300    | -15 to -25     |


*/
struct composition {
    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int duration          = 60 * 60 * fs_i;
    int channel_count_out = 2;

    float fdl_length_f = fs / 100;
    size_t fdl_length  = size_t(fdl_length_f + 1);

    float fundamental         = 110;  // hz
    float fundamental_decay   = 600;  // ms
    float fundamental_gain_db = -3;   // db

    const static int num_resonators = 5;
    float resonator_freqs[num_resonators]{
            fundamental * 1.00f,  //
            fundamental * 1.59f,
            fundamental * 2.14f,
            fundamental * 2.30f,
            fundamental * 2.65f,
    };

    float resonator_gains[num_resonators]{
            db_to_linear(fundamental_gain_db - 0),  //
            db_to_linear(fundamental_gain_db - 3),
            db_to_linear(fundamental_gain_db - 6),
            db_to_linear(fundamental_gain_db - 10),
            db_to_linear(fundamental_gain_db - 15),
    };
    float resonator_decay_coeffs[num_resonators]{
            0.959f,
            0.950f,
            0.96f,
            0.951f,
            0.95f,
    };
    float global_decay_damper = 1;

    resonator_section resonators[num_resonators]{
            {fs, 48000}, {fs, 48000}, {fs, 48000}, {fs, 48000}, {fs, 48000}};

    struct resonator_smooth {
        smooth cutoff_lpf;
        smooth cutoff_hpf;
        smooth freq;
        smooth gain_fb;
        smooth gain_out;

        void tick() {
            cutoff_lpf.tick();
            cutoff_hpf.tick();
            freq.tick();
            gain_fb.tick();
            gain_out.tick();
        }
    };

    resonator_smooth smoother[num_resonators];

    composition() : trigger_bottom(fs), trigger_noise(fs) {
        trigger_bottom.freq(resonator_freqs[0] * 1.999f);
        trigger_bottom.waveform = wave_square;
        trigger_noise.waveform  = wave_noise;

        for (auto i : std::views::iota(0, num_resonators)) {
            resonators[i].lpf_set(resonator_freqs[i] * 5, 0.707);
            resonators[i].hpf_set(30, 0.707);

            smoother[i].cutoff_lpf.callback = [this, i](float val) { this->resonators[i].lpf_cut(val); };
            smoother[i].cutoff_lpf.init(resonator_freqs[i] * 5);

            smoother[i].cutoff_hpf.callback = [this, i](float val) { resonators[i].hpf_cut(val); };
            smoother[i].cutoff_hpf.init(30);

            smoother[i].freq.callback = [this, i](float val) { resonators[i].freq(val); };
            smoother[i].freq.init(resonator_freqs[i]);

            smoother[i].gain_fb.callback = [this, i](float val) { resonators[i].m_gain_fb = val; };
            smoother[i].gain_fb.init(resonator_decay_coeffs[i]);

            smoother[i].gain_out.callback = [this, i](float val) { resonators[i].m_gain_out = val; };
            smoother[i].gain_out.init(resonator_gains[i]);
        }
    }

    oscillator trigger_bottom;
    oscillator trigger_noise;
    env_linear trigger_amp_env;
    float trigger_amp_env_decay = 0.0006f * fs;  // ms
    void trigger(float impulse_gain) {
        trigger_amp_env.set(impulse_gain, impulse_gain - 1, trigger_amp_env_decay);
        trigger_bottom.phase(0);
    };

    int_fast64_t counter      = 0;
    int_fast64_t snare_key_on = 24500;

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        float trigger_env = std::max(0.f, trigger_amp_env.tick());
        trigger_env       = tension_pos_a(trigger_env, 0.4);
        float trigger_signal =
                0.8f * trigger_env * (0.8f * trigger_bottom.tick() + (0.5f * trigger_noise.tick()));

        if (counter == snare_key_on) {
            trigger(1);
            trigger_bottom.phase(0);
        }

        if (++counter == 48000) {
            counter = 0;
        }

        float output = 0;

        for (auto i : std::views::iota(0, num_resonators)) {
            smoother[i].tick();
            output += resonators[i].tick(trigger_signal);
        }

        L = output;
        R = L;

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
