#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <bits/ranges_algo.h>
#include <bits/sat_arith.h>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <iterator>
#include <limits>
#include <ranges>
#include <span>
#include <stdexcept>

#include <print>

template <typename T>
concept frsq_data_base = requires(T t) {
    { t.start_time } -> std::same_as<double&>;
    { t.duration } -> std::same_as<double&>;
};

struct frsq_base {
    virtual void tick()  = 0;
    virtual ~frsq_base() = default;
};

template <typename voice_t, frsq_data_base frsq_data_t>
struct frsq : public frsq_base {
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

    bool loop_wrap_pending     = false;
    bool initial_pattern_start = false;

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
    }
    void set_pattern(std::span<frsq_data_t> new_pattern_data, double from_time_relative) {
        pattern_data = new_pattern_data;
        set_from_time(from_time_relative);
    }
    void set_pattern(std::span<frsq_data_t> new_pattern_data) {
        pattern_data = new_pattern_data;
        determine_last_event();
    }
    void determine_last_event() {
        if (!pattern_data.size()) {
            last_event = pattern_data.end();
            return;
        }

        double current_time_relative =
                (std::floor(current_time_absolute) / duration_absolute) * duration_relative;
        auto next_event = std::ranges::lower_bound(
                pattern_data, current_time_relative, std::ranges::less{}, &frsq_data_t::start_time);

        if (next_event != pattern_data.begin()) {
            last_event            = next_event - 1;
            initial_pattern_start = false;
        } else {
            last_event            = pattern_data.end() - 1;
            initial_pattern_start = true;
        }
    }

    void tick() final {
        // no voices
        if (voices.empty())
            throw std::runtime_error("no voices assigned to frsq");

        // too many voices
        if (voices.size() > voices_time_remaining.size()) {
            throw std::runtime_error("too many voices assigned to frsq");
        }

        for (auto i : std::views::iota(0, int(max_polyphony))) {
            voice_t& voice = voices[i];
            int& t_remain  = voices_time_remaining[i];
            int& t_elapsed = voices_time_elapsed[i];

            // time to end a voice
            if (t_remain == 0) {
                callback_end(voice);
            }

            // incr/decr counters
            if (t_remain > std::numeric_limits<int>::min())
                t_remain -= 1;
            if (t_elapsed < std::numeric_limits<int>::max())
                t_elapsed += 1;
        }

        // no pattern
        if (pattern_data.empty()) {
            return;
        }

        // make sure the last event is valid
        if (last_event == pattern_data.end())
            throw std::runtime_error("frsq last_event should never be pattern_data.end() at this point");
        if (!(last_event >= pattern_data.begin() && last_event < pattern_data.end())) {
            throw std::runtime_error(std::format(
                    "frsq {} last_event should always be between .begin() and .end()\n end distance: {}\ncur "
                    "distance: {}",
                    size_t(this),
                    std::distance(pattern_data.begin(), pattern_data.end()),
                    std::distance(pattern_data.begin(), last_event)));
        }

        // find next event
        auto next_event = last_event + 1;

        // wrap back around to the beginning if needed
        if (initial_pattern_start) {
            next_event            = pattern_data.begin();
            initial_pattern_start = false;
        } else if (next_event == pattern_data.end()) {
            if (!(current_time_absolute >= next_event->start_time)) {
                loop_wrap_pending = true;
            }
            next_event = pattern_data.begin();
        }

        // single event in pattern
        if (last_event == next_event) {
            throw std::runtime_error("frsq unhandled scenario: single event in a pattern.");
        }

        int next_event_time_absolute = int((next_event->start_time / duration_relative) * duration_absolute);

        // <= is used to account for when caller manipulates next_event::start_time in real time
        // because it could jump behind the frsq discrete time play head.
        bool is_event_start =
                !loop_wrap_pending && int(next_event_time_absolute) <= int(current_time_absolute);

        int events_this_sample = 0;
        while (is_event_start) {
            ++events_this_sample;

            // find oldest inactive voice
            voice_t* selected_voice = nullptr;
            int selected_index      = -1;
            auto times_zip          = std::views::zip(
                    std::views::iota(0, int(voices.size())), voices_time_remaining, voices_time_elapsed);

            if (voices.size() == 1) {
                selected_voice = &voices[0];
                selected_index = 0;
            } else {
                auto min_remain_element = std::ranges::min_element(
                        times_zip, [](auto lhs, auto rhs) { return std::get<1>(lhs) < std::get<1>(rhs); });
                auto min_elapsed_element = std::ranges::min_element(
                        times_zip, [](auto lhs, auto rhs) { return std::get<2>(lhs) < std::get<2>(rhs); });
                int min_remain_value  = std::get<1>(*min_remain_element);
                int min_elapsed_value = std::get<2>(*min_elapsed_element);

                bool has_inactive_voice = min_remain_value <= 0;
                if (has_inactive_voice) {
                    int i          = std::get<0>(*min_remain_element);
                    selected_voice = &voices[i];
                    selected_index = i;
                } else {
                    int i          = std::get<0>(*min_elapsed_element);
                    selected_voice = &voices[i];
                    selected_index = i;
                }
            }

            int& t_remain  = voices_time_remaining[selected_index];
            int& t_elapsed = voices_time_elapsed[selected_index];

            t_remain  = int((next_event->duration / duration_relative) * duration_absolute);
            t_elapsed = 0;

            callback_start(*selected_voice, *next_event);

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
