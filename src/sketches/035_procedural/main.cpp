// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <array>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <vector>

#include "clover/audio_buffer.hpp"
#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/wave.hpp"
#include "clover/io/audio_file.hpp"
#include "clover/math.hpp"

#include "notes.hpp"

struct pattern_event {
    float note;
    float start_time;
    float duration;
};

constexpr std::array beep_pattern{
    pattern_event{-36 + note::D4, 0, 1},
    pattern_event{-36 + note::D4, 1, 1},
    pattern_event{-36 + note::D4, 2, 1},
    pattern_event{-36 + note::D4, 3, 1},
};

auto beep_pattern_offset(float start_offset) {
    return beep_pattern | std::views::transform([start_offset](const auto event) {
               float s = event.start_time + start_offset;
               return pattern_event{event.note, event.start_time + start_offset, event.duration};
           });
}

auto beep_arrangement() {
    return std::views::iota(0, 64) |
           std::views::transform([](int i) { return beep_pattern_offset(float(i * 4)); }) | std::views::join;
}

constexpr size_t NUM_VOICES = 4;
auto beep_arrangements =
    std::views::iota(0u, NUM_VOICES) | std::views::transform([](size_t i) {
        auto i_f           = float(i);
        float pitch_offset = std::floor(i_f / 2.f) * 12.f;
        float start_offset = i_f / 4.f;
        return beep_arrangement() | std::views::transform([pitch_offset, start_offset](pattern_event event) {
                   return pattern_event{
                       event.note + pitch_offset, event.start_time + start_offset, event.duration};
               });
    });

auto get_arrangements_in_samples(float samples_per_beat) {
    return beep_arrangements | std::views::transform([samples_per_beat](auto arrangement) {
               return arrangement | std::views::transform([samples_per_beat](auto event) {
                          return pattern_event{
                              event.note,
                              (event.start_time * samples_per_beat),
                              (event.duration * samples_per_beat)};
                      });
           });
}

auto get_start_times(auto arrangements) {
    return arrangements | std::views::transform([](auto arrangement) {
               return arrangement |
                      std::views::transform([](auto event) { return size_t(event.start_time); });
           });
}
auto get_end_times(auto arrangements) {
    return arrangements | std::views::transform([](auto arrangement) {
               return arrangement | std::views::transform(
                                        [](auto event) { return size_t(event.start_time + event.duration); });
           });
}

int main(int, char*[]) {
    constexpr float bpm    = 160;
    constexpr float fs     = 48000;
    constexpr float spm    = fs * bpm;
    constexpr float spbeat = spm / bpm;
    constexpr float spbar  = spbeat * 4;

    constexpr float duration_bars = 16 * 4;
    constexpr float duration      = spbar * duration_bars;
    constexpr auto duration_z     = size_t(duration);
    constexpr auto clock          = std::views::iota(0u, duration_z);

    auto arrangements = get_arrangements_in_samples(spbeat) | std::ranges::to<std::vector>();
    auto start_times  = get_start_times(arrangements) | std::ranges::to<std::vector>();
    auto end_times    = get_end_times(arrangements) | std::ranges::to<std::vector>();

    std::vector<float> output_signal;
    output_signal.resize(duration_z, 0);

    for (auto vi : std::views::iota(0u, NUM_VOICES)) {
        auto start_time_vi = start_times[vi] | std::ranges::to<std::vector>();
        auto end_time_vi   = end_times[vi] | std::ranges::to<std::vector>();
        auto arrangement   = arrangements[vi] | std::ranges::to<std::vector>();

        // std::println(
        //     "{} {} {}", start_time_vi.size(), end_time_vi.size(), arrangement.size()
        // );
        // for (auto [x, start, end] : std::views::zip(arrangement, start_time_vi, end_time_vi)) {
        //     std::println("{} {} {} - {} {}", x.note, x.start_time, x.duration, start, end);
        // }

        clover::dsp::env_adsr beep_amp_adsr;
        beep_amp_adsr.set(100, spbeat * 0.2, 0.f, 100);
        std::vector<float> beep_amp_adsr_signal;
        beep_amp_adsr_signal.reserve(duration_z);

        for (auto i : clock) {
            if (std::ranges::find(start_time_vi, i) != start_time_vi.end()) {
                beep_amp_adsr.key_on();
            } else if (std::ranges::find(end_time_vi, i) != end_time_vi.end()) {
                beep_amp_adsr.key_off();
            }

            beep_amp_adsr_signal.emplace_back(beep_amp_adsr.tick());
        }

        clover::dsp::env_adsr beep_pitch_adsr;
        beep_pitch_adsr.set(100, spbeat / 32, 0.f, 100);
        std::vector<float> beep_pitch_adsr_signal;
        beep_pitch_adsr_signal.reserve(duration_z);

        for (auto i : clock) {
            if (std::ranges::find(start_time_vi, i) != start_time_vi.end()) {
                beep_pitch_adsr.key_on();
            } else if (std::ranges::find(end_time_vi, i) != end_time_vi.end()) {
                beep_pitch_adsr.key_off();
            }

            const float beep_pitch_mod_octaves = 2;
            beep_pitch_adsr_signal.emplace_back(beep_pitch_adsr.tick() * beep_pitch_mod_octaves);
        }

        std::vector<float> beep_osc_signal;
        beep_osc_signal.reserve(duration_z);

        clover::dsp::oscillator sine_osc{fs};
        sine_osc.waveform = clover::dsp::wave_sine;

        float current_freq = 0;
        for (auto i : clock) {
            auto it_start = std::ranges::find(start_time_vi, i);
            if (it_start != start_time_vi.end()) {
                auto idx   = std::ranges::distance(start_time_vi.begin(), it_start);
                auto event = arrangement[idx];
                sine_osc.phase(0);
                current_freq = clover::midi_to_frequency(event.note);
            }

            float freq = clover::frequency_by_octave_difference(current_freq, beep_pitch_adsr_signal[i]);
            freq       = std::min(freq, 15000.f);
            sine_osc.freq(freq);

            beep_osc_signal.emplace_back(sine_osc.tick());
        }
        for (auto i : clock) {
            beep_osc_signal[i] *= beep_amp_adsr_signal[i] * 0.1f;
        }
        for (auto i : clock) {
            beep_osc_signal[i] = std::tanh(4.5f * beep_osc_signal[i]);
        }
        for (auto i : clock) {
            output_signal[i] += beep_osc_signal[i];
        }
    }

    clover::audio_buffer buffer;
    buffer.channels    = 2;
    buffer.sample_rate = int(fs);
    buffer.data        = output_signal;

    clover::convert_sample_rate(buffer, 44100);
    clover::io::audio_file::write("035_procedural.wav", buffer, clover::io::audio_file_settings::wav_441_16);

    return 0;
}
