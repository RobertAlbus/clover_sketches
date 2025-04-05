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
#include <cstdint>
#include <filesystem>
#include <format>
#include <functional>
#include <iterator>
#include <limits>
#include <ranges>
#include <span>
#include <stdexcept>

#include <fstream>
#include <print>
#include <type_traits>

// troubleshooting logging;
inline static const char* individual_log_path_partial = "/home/ra/clover/sketches/src/019_composition/log__";

template <typename T>
concept frsq_data_base = requires(T t) {
    { t.start_time } -> std::same_as<double&>;
    { t.duration } -> std::same_as<double&>;
};

// TODO: WHY DOES IT APPEAR THAT FRSQ SOMETIMES DOUBLE TRIGGERS OR DOUBLE ENDS?
// is this an issue with println buffering, or is it actually double-eventing?

template <typename voice_t, frsq_data_base frsq_data_t>
struct frsq {
    std::ofstream frsq_log;
    int64_t global_time = 0;
    frsq(bool enable_logging = false) {
        // if (enable_logging) {
        //     // std::println("enabled logging");
        //     auto path = std::format(
        //             "/home/ra/clover/sketches/src/sketches/019_composition/sequence/logs/log__{}.log",
        //             static_cast<void*>(this));
        //     std::filesystem::create_directories(std::filesystem::path(path).parent_path());
        //     frsq_log.open(path, std::ios::trunc);
        // }

        voices_time_remaining.fill(std::numeric_limits<int>::min());
        voices_time_elapsed.fill(std::numeric_limits<int>::max());
        // std::println(frsq_log, "frsq constructed");

        frsq_log.flush();
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
        // std::println(frsq_log, "frsq.set_from_time(from_time_relative)");
        // std::println(frsq_log, "    from relative t: {}", from_time_relative);

        // std::println(frsq_log, "    fmod({}, {})", from_time_relative, duration_relative);
        from_time_relative = std::fmod(from_time_relative, duration_relative);
        // std::println(frsq_log, "    : {}", from_time_relative);

        if (from_time_relative < 0)
            from_time_relative += duration_relative;

        // std::println(frsq_log, "    : {}", from_time_relative);

        current_time_absolute = (from_time_relative / duration_relative) * duration_absolute;
        // std::println(frsq_log, "    current_time_absolute: {}", current_time_absolute);
        determine_last_event();

        // TODO: handle loop_wrap_pending
        frsq_log.flush();
    }
    void set_pattern(std::span<frsq_data_t> new_pattern_data, double from_time_relative) {
        // std::println(frsq_log, "frsq.set_pattern(new_pattern_data, from_time_relative)");
        // std::println(frsq_log, "    size of pattern: {}", new_pattern_data.size());
        // std::println(frsq_log, "    from relative t: {}", from_time_relative);

        pattern_data = new_pattern_data;
        set_from_time(from_time_relative);

        // TODO: handle loop_wrap_pending
        frsq_log.flush();
    }
    void set_pattern(std::span<frsq_data_t> new_pattern_data) {
        // std::println(frsq_log, "frsq.set_pattern(new_pattern_data)");
        // std::println(frsq_log, "    size of pattern: {}", new_pattern_data.size());
        pattern_data = new_pattern_data;
        determine_last_event();

        // TODO: handle loop_wrap_pending
        frsq_log.flush();
    }
    void determine_last_event() {
        // std::println(frsq_log, "frsq.determine_last_event()");
        // std::println(frsq_log, "    pattern size: {}", pattern_data.size());

        // TODO: handle loop_wrap_pending
        // maybe it should go here
        if (!pattern_data.size()) {
            last_event = pattern_data.end();
            // std::println(frsq_log, "    escape hatch");
            return;
        }

        // std::floor fixes bug with missing the first pluck note when when going from pattern 3 back to
        // pattern 1
        double current_time_relative =
                (std::floor(current_time_absolute) / duration_absolute) * duration_relative;
        // std::println(frsq_log, "    current time abs: {}", current_time_absolute);
        // std::println(frsq_log, "    current time rel: {}", current_time_relative);
        // std::println(frsq_log, "    duration abs:     {}", duration_absolute);
        // std::println(frsq_log, "    duration rel:     {}", duration_relative);

        // maybe the logic here is borked?
        // std::println(frsq_log, "");
        auto next_event = std::ranges::lower_bound(
                pattern_data, current_time_relative, std::ranges::less{}, &frsq_data_t::start_time);

        if (next_event != pattern_data.begin()) {
            // std::println(frsq_log, "    next event != .begin()");
            last_event            = next_event - 1;
            initial_pattern_start = false;
        } else {
            // std::println(frsq_log, "    next event == .begin()");
            last_event            = pattern_data.end() - 1;
            initial_pattern_start = true;
        }
        frsq_log.flush();
    }

    void tick() {
        // std::println(frsq_log, "\n\nfrsq.tick()");
        // std::println(frsq_log, "TICK BEGIN --------------------------------");
        // std::println(frsq_log, "    global time: {}", global_time++);
        // std::println(frsq_log, "    current abs: {}", current_time_absolute);

        // std::println(frsq_log, "");

        // no voices
        if (voices.empty())
            throw std::runtime_error("no voices assigned to frsq");

        // too many voices
        if (voices.size() > voices_time_remaining.size()) {
            throw std::runtime_error("too many voices assigned to frsq");
        }

        // std::println(frsq_log, "    tr | te ");
        for (auto [tr, te] : std::views::zip(voices_time_remaining, voices_time_elapsed)) {
            // std::println(frsq_log, "    {} | {} ", tr, te);
        }
        auto voices_zip = std::views::zip(voices_time_remaining, voices_time_elapsed);
        // auto voices_zip = std::views::zip(voices, voices_time_remaining, voices_time_elapsed);
        for (auto i : std::views::iota(0, int(max_polyphony))) {
            voice_t& voice = voices[i];
            int& t_remain  = voices_time_remaining[i];
            int& t_elapsed = voices_time_elapsed[i];
            // // std::println(frsq_log, "");
            // // std::println(frsq_log, "    for each item in voices_zip");
            // // std::println(frsq_log, "        tr {} : te {}", t_remain, t_elapsed);

            // if constexpr (std::is_same<decltype(t_remain), int&>()) {
            //     // std::println(frsq_log, "    IS TYPE int&");
            // } else if constexpr (std::is_same<decltype(t_remain), int*>()) {
            //     // std::println(frsq_log, "    IS TYPE int*");
            // } else if constexpr (std::is_same<decltype(t_remain), int>()) {
            //     // std::println(frsq_log, "    IS TYPE int");
            // }
            // // std::println(
            //         frsq_log,
            //         "    voice zip indices:  {} {} {}",
            //         std::distance(voices.begin(), voice),
            //         std::distance(voices_time_remaining.begin(), *t_remain),
            //         std::distance(voices_time_elapsed.begin(), *t_elapsed));

            // time to end a voice
            if (t_remain == 0) {
                // // std::println(frsq_log, "    invoking callback_end()");
                callback_end(voice);
            }

            // incr/decr counters
            if (t_remain > std::numeric_limits<int>::min())
                t_remain -= 1;
            if (t_elapsed < std::numeric_limits<int>::max())
                t_elapsed += 1;

            // // std::println(frsq_log, "        tr {} : te {}", t_remain, t_elapsed);
            // // std::println(frsq_log, "");
        }

        // no pattern
        if (pattern_data.empty()) {
            // std::println(frsq_log, "    empty pattern => return");
            // std::println(frsq_log, "TICK END -----------------------");

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
        // std::println(frsq_log, "    loop wrap pending: {}", loop_wrap_pending);
        if (initial_pattern_start) {
            next_event            = pattern_data.begin();
            initial_pattern_start = false;
        } else if (next_event == pattern_data.end()) {
            // std::println(frsq_log, "    ...reached end");
            if (current_time_absolute >= next_event->start_time) {
                // std::println(frsq_log, "    loop wrap executed");
            } else {
                loop_wrap_pending = true;
                // std::println(frsq_log, "    loop wrap pending: {}", loop_wrap_pending);
            }
            next_event = pattern_data.begin();
        }

        // single event in pattern
        if (last_event == next_event) {
            throw std::runtime_error("frsq unhandled scenario: single event in a pattern.");
        }

        // std::println(frsq_log, "");
        int next_event_time_absolute = int((next_event->start_time / duration_relative) * duration_absolute);

        // clang-format off
        // std::println(frsq_log, "    determining next event start (A)");
        // std::println(frsq_log, "        next event start rel: {}/{}", next_event->start_time, duration_relative);
        // std::println(frsq_log, "        next event abs:       {}", next_event_time_absolute);
        // std::println(frsq_log, "        current time abs:     {}", current_time_absolute);
        // std::println(frsq_log, "        next abs <= curr abs: {}", int(next_event_time_absolute) <= int(current_time_absolute));
        // std::println(frsq_log, "");
        // clang-format on

        // <= is used to account for when caller manipulates next_event::start_time in real time
        // because it could jump behind the frsq discrete time play head.
        bool is_event_start =
                !loop_wrap_pending && int(next_event_time_absolute) <= int(current_time_absolute);

        // std::println(frsq_log, "");
        // std::println(frsq_log, "    is event start: {}", is_event_start);

        // std::println(frsq_log, "");
        int events_this_sample = 0;
        while (is_event_start) {
            ++events_this_sample;
            // std::println(frsq_log, "    event: {}", events_this_sample);

            // find oldest inactive voice
            voice_t* selected_voice = nullptr;
            int selected_index      = -1;
            auto times_zip          = std::views::zip(
                    std::views::iota(0, int(voices.size())), voices_time_remaining, voices_time_elapsed);

            if (voices.size() == 1) {
                selected_voice = &voices[0];
                selected_index = 0;

                // std::println(frsq_log, "    monophonic: selected only voice");
            } else {
                // std::println(frsq_log, "    polyphonic: find most suitable voice");

                auto min_remain_element = std::ranges::min_element(
                        times_zip, [](auto lhs, auto rhs) { return std::get<1>(lhs) < std::get<1>(rhs); });
                auto min_elapsed_element = std::ranges::min_element(
                        times_zip, [](auto lhs, auto rhs) { return std::get<2>(lhs) < std::get<2>(rhs); });
                int min_remain_value  = std::get<1>(*min_remain_element);
                int min_elapsed_value = std::get<2>(*min_elapsed_element);
                //clang-format off
                // std::println(frsq_log,"        min remain  value: voices_time_remaining[{}]:
                // {}",std::get<0>(*min_remain_element),min_remain_value); std::println(frsq_log,"        min
                // elapsed value: voices_time_elapsed[{}]:
                // {}",std::get<0>(*min_elapsed_element),min_elapsed_value);
                //clang-format on

                bool has_inactive_voice = min_remain_value <= 0;
                if (has_inactive_voice) {
                    int i = std::get<0>(*min_remain_element);
                    // std::println(frsq_log, "        has inactive voice: voices[{}]", i);
                    selected_voice = &voices[i];
                    selected_index = i;
                } else {
                    int i = std::get<0>(*min_elapsed_element);
                    // std::println(frsq_log, "        stealing voice:     voices[{}]", i);
                    selected_voice = &voices[i];
                    selected_index = i;
                }
            }

            int& t_remain  = voices_time_remaining[selected_index];
            int& t_elapsed = voices_time_elapsed[selected_index];

            // std::println(frsq_log, "");
            // std::println(frsq_log, "    updating times for item");
            // clang-format off
            // std::println(frsq_log,"        [i, t_remain, t_elapsed] : [{}, {}, {}]",selected_index,t_remain,t_elapsed);

                    t_remain  = int((next_event->duration / duration_relative) * duration_absolute);
                    t_elapsed = 0;

                    // std::println(frsq_log,"        [i, t_remain, t_elapsed] : [{}, {}, {}]",selected_index,t_remain,t_elapsed);
            // clang-format on

            // std::println(frsq_log, "    callback_start()");
            callback_start(*selected_voice, *next_event);

            last_event = next_event;

            if (++next_event == pattern_data.end()) {
                // std::println(frsq_log, "    ++next_event == pattern_data.end()");
                loop_wrap_pending = true;
                next_event        = pattern_data.begin();
            }
            next_event_time_absolute = int((next_event->start_time / duration_relative) * duration_absolute);

            // <= is used to account for when caller manipulates next_event::start_time in real time
            // because it could jump behind the frsq discrete time play head.
            is_event_start = !loop_wrap_pending && next_event_time_absolute <= int(current_time_absolute);

            // clang-format off
            // std::println(frsq_log, "    determining next event start (B)");
            // std::println(frsq_log, "        loop wrap pending:    {}", loop_wrap_pending);
            // std::println(frsq_log, "        next event start rel: {}/{}", next_event->start_time, duration_relative);
            // std::println(frsq_log, "        next event abs:       {}", next_event_time_absolute);
            // std::println(frsq_log, "        current time abs:     {}", current_time_absolute);
            // std::println(frsq_log, "        next abs <= curr abs: {}", int(next_event_time_absolute) <= int(current_time_absolute));
            // std::println(frsq_log, "");
            // std::println(frsq_log, "    is event start: {}", is_event_start);
            // clang-format on
        }

        current_time_absolute += 1;
        if (current_time_absolute >= duration_absolute) {
            loop_wrap_pending = false;
            current_time_absolute -= duration_absolute;
        }

        // std::println(frsq_log, "TICK END -----------------------");
        frsq_log.flush();
    }
};
