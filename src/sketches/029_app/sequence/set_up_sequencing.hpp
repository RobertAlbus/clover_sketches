#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <memory>
#include <print>
#include <stdexcept>
#include <vector>

#include "lib/logging/logger.hpp"
#include "lib/sq/frsq_024.hpp"

#include "infrastructure/bar_grid.hpp"

#include "event.hpp"
#include "sequence/patterns.hpp"
#include "sequence/sequencers.hpp"

template <typename voice_t, typename event_t>
frsq_pair create_sequencers(
        std::span<voice_t> voices,
        typename frsq_024<voice_t, event_t>::callback_start_t callback_start,
        typename frsq_024<voice_t, event_t>::callback_end_t callback_end,
        log_bus_000& logger,
        bar_grid& grid,
        std::vector<pattern_t<event_t>>& patterns,
        std::vector<event_meta_sq>& arrangement,
        double duration_bars,
        double duration_bars_in_samples,
        std::string logging_name) {
    auto sq      = std::make_unique<frsq_024<voice_t, event_t>>();
    auto meta_sq = std::make_unique<frsq_024<frsq_024<voice_t, event_t>, event_meta_sq>>();

    set_up_sequencing(
            voices,
            *sq,
            callback_start,
            callback_end,
            *meta_sq,
            logger,
            grid,
            patterns,
            arrangement,
            duration_bars,
            duration_bars_in_samples,
            logging_name);

    return {std::move(sq), std::move(meta_sq)};
}

// this assumes one sequencer per meta sequencer.
// this has been true *so far*
// but I could want to control multiple sequencers from a single meta sequencer!
template <typename voice_t, typename event_t>
void set_up_sequencing(
        std::span<voice_t> voices,
        frsq_024<voice_t, event_t>& sq,
        typename frsq_024<voice_t, event_t>::callback_start_t callback_start,
        typename frsq_024<voice_t, event_t>::callback_end_t callback_end,
        frsq_024<frsq_024<voice_t, event_t>, event_meta_sq>& meta_sq,
        log_bus_000& logger,
        bar_grid& grid,
        std::vector<pattern_t<event_t>>& patterns,
        std::vector<event_meta_sq>& arrangement,
        double duration_bars,
        double duration_bars_in_samples,
        std::string logging_name) {
    sq.voices         = voices;
    sq.callback_start = callback_start;
    sq.callback_end   = callback_end;

    meta_sq.voices         = std::span(&sq, 1);
    meta_sq.callback_start = arrangement_callback_for(meta_sq, logger, grid, patterns, logging_name);
    meta_sq.set_pattern(arrangement, duration_bars_in_samples, duration_bars, 0);
}

template <typename voice_t, frsq_data_base_000 event_t>
std::function<void(frsq_024<voice_t, event_t>&, const event_meta_sq&)> arrangement_callback_for(
        frsq_024<frsq_024<voice_t, event_t>, event_meta_sq>& meta_frsq,
        log_bus_000& logger,
        bar_grid& grid,
        std::vector<pattern_t<event_t>>& patterns,
        std::string logging_name) {
    return [&meta_frsq, &logger, logging_name, &patterns, &grid](
                   frsq_024<voice_t, event_t>& voice, const event_meta_sq& event_meta) mutable {
        log_message_000 msg;
        snprintf(
                msg.text,
                sizeof(msg.text),
                " - %-16s %zu @ %f",
                logging_name.c_str(),
                event_meta.pattern_index,
                event_meta.start_time);
        logger.gui.try_enqueue(msg);

        if (event_meta.pattern_index >= patterns.size())
            throw std::runtime_error(std::format(
                    "selected non-existant pattern[{}] for {}", event_meta.pattern_index, logging_name));

        double pattern_duration_relative = patterns[event_meta.pattern_index].duration_rel;
        double pattern_duration_bars     = patterns[event_meta.pattern_index].duration_bar;
        double pattern_duration_absolute = grid.bars_to_samples(pattern_duration_bars);

        double current_time_meta_frsq = meta_frsq.current_time_absolute;
        double start_point            = std::fmod(current_time_meta_frsq, pattern_duration_absolute) /
                             pattern_duration_absolute * pattern_duration_relative;
        voice.set_pattern(
                patterns[event_meta.pattern_index].pattern,
                pattern_duration_absolute,
                pattern_duration_relative,
                start_point);
    };
}
