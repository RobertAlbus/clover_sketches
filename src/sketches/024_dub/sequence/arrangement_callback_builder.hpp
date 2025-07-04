#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <print>
#include <stdexcept>
#include <vector>

#include "lib/logging/logger.hpp"
#include "lib/sq/frsq_024.hpp"

#include "lib/sq/bar_grid.hpp"

#include "event.hpp"
#include "sequence/patterns.hpp"

template <typename voice_t, frsq_data_base_000 event_t>
std::function<void(frsq_024<voice_t, event_t>&, const event_meta_sq&)> arrangement_callback_for(
        log_bus_000& logger,
        bar_grid& grid,
        std::vector<pattern_t<event_t>>& patterns,
        std::string logging_name) {
    return [&logger, logging_name, &patterns, &grid](
                   frsq_024<voice_t, event_t>& voice, const event_meta_sq& event) mutable {
        log_message_000 msg;
        snprintf(
                msg.text,
                sizeof(msg.text),
                " - %-16s %zu @ %f",
                logging_name.c_str(),
                event.pattern_index,
                event.start_time);
        logger.gui.try_enqueue(msg);

        if (event.pattern_index >= patterns.size())
            throw std::runtime_error(
                    std::format(
                            "selected non-existant pattern[{}] for {}", event.pattern_index, logging_name));
        voice.set_pattern(
                patterns[event.pattern_index].pattern,
                grid.bars_to_samples(patterns[event.pattern_index].duration_bar),
                patterns[event.pattern_index].duration_rel,
                0);
    };
}

// template <typename voice_t, frsq_data_base event_t>
// std::function<void(frsq_024<voice_t, event_t>& voice, const event_meta_sq& event)> callback_for(
//         frsq_024<frsq_024<voice_t, event_t>, event_meta_sq>& meta_frsq,
//         frsq_024<voice_t, event_t>& voice,
//         logger** logger,
//         std::vector<std::vector<event_t>> patterns, std::string logging_name) {
//     return [logger, logging_name, patterns](
//                    frsq_024<voice_t, event_t>& voice, const event_meta_sq& event) mutable {
//         if (logger && *logger) {
//             gui_log_message msg;
//             snprintf(
//                     msg.text,
//                     sizeof(msg.text),
//                     " - %-16s %zu @ %f",
//                     logging_name.c_str(),
//                     event.pattern_index,
//                     event.start_time);
//             bool sent = (*logger)->gui.try_enqueue(msg);
//             if (!sent) {
//                 std::fprintf(
//                         stderr, "failed to log to gui: arrangement callback for %s\n",
//                         logging_name.c_str());
//             }
//         }
//         voice.set_pattern(patterns[event.pattern_index]);
//     };
// }