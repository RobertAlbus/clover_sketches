#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <bits/ranges_algo.h>
#include <bits/sat_arith.h>
#include <cmath>
#include <cstdint>
#include <format>
#include <functional>
#include <iterator>
#include <limits>
#include <ranges>
#include <span>
#include <stdexcept>

#include "lib/debug_utils/demangle_type_name.hpp"

#include "frsq_000.hpp"

struct frsq_base_024 {
    virtual ~frsq_base_024()                         = default;
    virtual void choke_all()                         = 0;
    virtual void trigger_most_recent_event()         = 0;
    virtual void set_time(double from_time_relative) = 0;
    virtual void tick()                              = 0;
};

template <typename voice_t, frsq_data_base_000 frsq_data_t>
struct frsq_024 : public frsq_base_024 {
    frsq_024(const frsq_024&) = delete;
    frsq_024() {
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

    double duration_absolute = 0;  // in samples
    double duration_relative = 0;  // arbitrary musical reference duration

    int64_t current_time_absolute         = 0;  // in samples
    double current_time_absolute_fraction = 0;  // arbitrary musical reference duration

    using callback_start_t          = std::function<void(voice_t& voice, const frsq_data_t& data)>;
    callback_start_t callback_start = [](voice_t& voice, const frsq_data_t& data) {};

    using callback_end_t        = std::function<void(voice_t& voice)>;
    callback_end_t callback_end = [](voice_t& voice) {};

    void choke_all() override {
        for (auto& voice : voices)
            callback_end(voice);
        voices_time_remaining.fill(std::numeric_limits<int>::min());
        voices_time_elapsed.fill(std::numeric_limits<int>::max());
    }

    void trigger_most_recent_event() override {
        callback_start(voices[0], *last_event);
    }

    // TODO: this is needed for generalizing the sequencing setup
    /*
        - base class frsq_base_024 with virtual methods
            - tick, choke, set_time etc
        - set up sequencers and meta sequencers once
        - sqs.start_from(float) just needs to meta_sq.set_time(from_time_relative)
            and avoids needing any type info
    */
    void set_time(double from_time_relative) override {
        from_time_relative = std::fmod(from_time_relative, duration_relative);
        if (from_time_relative < 0)
            from_time_relative += duration_relative;

        double current_time_samples    = (from_time_relative / duration_relative) * duration_absolute;
        current_time_absolute          = int64_t(current_time_samples);
        current_time_absolute_fraction = current_time_samples - current_time_absolute;
        determine_last_event();
    }
    void set_pattern(
            std::span<frsq_data_t> new_pattern_data,
            double new_duration_samples,
            double new_duration_relative,
            double from_time_relative) {
        duration_absolute = new_duration_samples;
        duration_relative = new_duration_relative;
        pattern_data      = new_pattern_data;

        set_time(from_time_relative);
    }

    double current_time_relative() {
        double current_time_abs = double(current_time_absolute) + current_time_absolute_fraction;
        return (current_time_abs / duration_absolute) * duration_relative;
    }

    void set_pattern(
            std::span<frsq_data_t> new_pattern_data,
            double new_duration_samples,
            double new_duration_relative) {
        set_pattern(new_pattern_data, new_duration_samples, new_duration_relative, current_time_relative());
    }

    void determine_last_event() {
        if (!pattern_data.size()) {
            last_event = pattern_data.end();
            return;
        }

        if (current_time_absolute == 0) {
            last_event = pattern_data.end() - 1;
            return;
        }

        auto next_event = std::ranges::lower_bound(
                pattern_data, current_time_relative(), std::ranges::less{}, &frsq_data_t::start_time);

        if (next_event == pattern_data.begin()) {
            last_event = pattern_data.end() - 1;
            return;
        }

        last_event = next_event - 1;
    }

    void tick() override {
        if (pattern_data.empty()) {
            return;
        }

        if (current_time_absolute_fraction >= 1) {
            current_time_absolute_fraction -= 1;
            return;
        }

        // no voices
        if (voices.empty())
            throw std::runtime_error("no voices assigned to frsq");

        // more voices than counters
        if (voices.size() > voices_time_remaining.size()) {
            throw std::runtime_error("too many voices assigned to frsq");
        }

        for (auto i : std::views::iota(0, int(max_polyphony))) {
            voice_t& voice = voices[i];
            int& t_remain  = voices_time_remaining[i];
            int& t_elapsed = voices_time_elapsed[i];

            if (t_remain == 0) {
                callback_end(voice);
            }

            if (t_remain > std::numeric_limits<int>::min())
                t_remain -= 1;
            if (t_elapsed < std::numeric_limits<int>::max())
                t_elapsed += 1;
        }

        // make sure the last event is valid
        if (last_event == pattern_data.end())
            throw std::runtime_error("frsq last_event should never be pattern_data.end() at this point");
        if (last_event < pattern_data.begin() || last_event >= pattern_data.end()) {
            throw std::runtime_error(
                    std::format(
                            "{} last_event should always be between .begin() and .end()\n"
                            "  end distance: {}\n  cur distance: {}",
                            demangle_type_name(this),
                            std::distance(pattern_data.begin(), pattern_data.end()),
                            std::distance(pattern_data.begin(), last_event)));
        }

        auto next_event = last_event + 1;
        if (next_event == pattern_data.end() && current_time_absolute == 0) {
            // finished all events in the pattern and playing again
            next_event = pattern_data.begin();

        } else if (next_event == pattern_data.end() && current_time_absolute > 0) {
            // finished all events in the pattern but not yet finished duration
            if (current_time_absolute >= ((last_event->start_time / duration_relative) * duration_absolute)) {
                increment_time();
                return;
            }
            next_event = pattern_data.begin();
        }

        int64_t next_event_time_absolute =
                int64_t((next_event->start_time / duration_relative) * duration_absolute);

        // <= is used to account for when caller manipulates next_event::start_time in real time
        // because it could jump behind the frsq discrete time play head.
        bool is_event_start = next_event_time_absolute <= current_time_absolute;

        while (is_event_start) {
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
                increment_time();
                return;
            }
            next_event_time_absolute = int((next_event->start_time / duration_relative) * duration_absolute);

            // <= is used to account for when caller manipulates next_event::start_time in real time
            // because it could jump behind the frsq discrete time play head.
            is_event_start = next_event_time_absolute <= current_time_absolute;
        }

        increment_time();
    }

    void increment_time() {
        current_time_absolute += 1;
        if (current_time_absolute >= duration_absolute) {
            auto truncated_duration = int64_t(duration_absolute);
            current_time_absolute_fraction += (duration_absolute - double(truncated_duration));
            current_time_absolute = 0;
        }
    }
};
