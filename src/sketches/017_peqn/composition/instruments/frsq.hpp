#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <bits/ranges_algo.h>
#include <bits/sat_arith.h>
#include <cmath>
#include <concepts>
#include <functional>
#include <limits>
#include <ranges>
#include <span>

template <typename T>
concept frsq_data_base = requires(T t) {
    { t.start_time } -> std::same_as<double&>;
    { t.duration } -> std::same_as<double&>;
};

template <frsq_data_base frsq_data_t, typename voice_t>
struct frsq {
    frsq() {
        voices_time_remaining.fill(std::numeric_limits<int>::min());
        voices_time_elapsed.fill(std::numeric_limits<int>::max());
    }

    static const int max_polyphony = 12;
    std::array<int, max_polyphony> voices_time_remaining{};
    std::array<int, max_polyphony> voices_time_elapsed{};
    std::span<voice_t> voices;
    std::span<frsq_data_t> pattern_data;  // assumed to be sorted
    using pattern_it = std::span<frsq_data_t>::iterator;
    pattern_it last_event;

    double duration_absolute     = 0;  // in samples
    double duration_relative     = 0;  // arbitrary musical reference duration
    double current_time_absolute = 0;  // in samples

    bool loop_wrap_pending = false;

    std::function<void(voice_t& voice, const frsq_data_t& data)> callback_start =
            [](voice_t& voice, const frsq_data_t& data) {};
    std::function<void(voice_t& voice)> callback_end = [](voice_t& voice) {};

    void choke_all() {
        for (auto& voice : voices)
            callback_end(voice);
        voices_time_remaining.fill(std::numeric_limits<int>::min());
        voices_time_elapsed.fill(std::numeric_limits<int>::max());
    }

    void set_from_time(double from_time_relative) {
        from_time_relative = std::fmod(from_time_relative, duration_relative);
        if (from_time_relative < 0)
            from_time_relative += duration_relative;

        current_time_absolute = (from_time_relative / duration_relative) * duration_absolute;
        determine_last_event();

        // TODO: handle loop_wrap_pending
    }
    void set_pattern(std::span<frsq_data_t> new_pattern_data, double from_time_relative) {
        pattern_data = new_pattern_data;
        set_from_time(from_time_relative);

        // TODO: handle loop_wrap_pending
    }
    void set_pattern(std::span<frsq_data_t> new_pattern_data) {
        pattern_data = new_pattern_data;
        determine_last_event();

        // TODO: handle loop_wrap_pending
    }
    void determine_last_event() {
        // TODO: handle loop_wrap_pending
        // maybe it should go here
        if (!pattern_data.size()) {
            last_event = pattern_data.end();
            return;
        }

        // std::floor fixes bug with missing the first pluck note when when going from pattern 3 back to
        // pattern 1
        double current_time_relative =
                (std::floor(current_time_absolute) / duration_absolute) * duration_relative;
        auto next_event = std::ranges::lower_bound(
                pattern_data, current_time_relative, std::ranges::less{}, &frsq_data_t::start_time);

        if (next_event != pattern_data.begin()) {
            last_event = next_event - 1;
        } else {
            last_event = pattern_data.end() - 1;
        }
    }

    void tick() {
        if (pattern_data.empty())
            return;

        auto voices_zip = std::views::zip(voices, voices_time_remaining, voices_time_elapsed);
        for (auto [voice, t_remain, t_elapsed] : voices_zip) {
            if (t_remain == 0) {
                callback_end(voice);
            }

            t_remain  = std::sub_sat(t_remain, 1);
            t_elapsed = std::add_sat(t_elapsed, 1);
        }

        auto next_event = last_event + 1;
        if (next_event == pattern_data.end())
            next_event = pattern_data.begin();

        int next_event_time_absolute = int((next_event->start_time / duration_relative) * duration_absolute);

        // <= is used to account for when caller manipulates next_event::start_time in real time
        // because it could jump behind the frsq discrete time play head.
        bool is_event_start =
                !loop_wrap_pending && int(next_event_time_absolute) <= int(current_time_absolute);
        int events_this_sample = 0;
        while (is_event_start) {
            ++events_this_sample;
            // find oldest inactive voice
            auto selected_voice = std::ranges::min_element(
                    voices_zip, [](auto lhs, auto rhs) { return std::get<1>(lhs) < std::get<1>(rhs); });

            bool has_inactive_voice = std::get<1>(*selected_voice) <= 0;
            if (!has_inactive_voice) {
                // steal oldest active voice
                selected_voice = std::ranges::min_element(
                        voices_zip, [](auto lhs, auto rhs) { return std::get<2>(lhs) < std::get<2>(rhs); });
            }
            auto [voice, t_remain, t_elapsed] = *selected_voice;

            t_remain  = int((next_event->duration / duration_relative) * duration_absolute);
            t_elapsed = 0;
            callback_start(voice, *next_event);

            last_event = next_event;

            if (++next_event == pattern_data.end()) {
                loop_wrap_pending = true;
                next_event        = pattern_data.begin();
            }
            next_event_time_absolute = int((next_event->start_time / duration_relative) * duration_absolute);

            // <= is used to account for when caller manipulates next_event::start_time in real time
            // because it could jump behind the frsq discrete time play head.
            is_event_start = !loop_wrap_pending && next_event_time_absolute <= int(current_time_absolute);
        }

        current_time_absolute += 1;
        if (current_time_absolute >= duration_absolute) {
            loop_wrap_pending = false;
            current_time_absolute -= duration_absolute;
        }
    }
};
