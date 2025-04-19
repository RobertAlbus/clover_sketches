#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <print>
#include <vector>

#include "concurrentqueue.h"

#include "infrastructure/logger.hpp"
#include "instruments/frsq.hpp"

#include "event.hpp"

template <typename voice_t, frsq_data_base event_t>
std::function<void(frsq<voice_t, event_t>& voice, const event_meta_sq& event)> callback_for(
        logger** logger, std::vector<std::vector<event_t>> patterns, std::string logging_name) {
    return [logger, logging_name, patterns](
                   frsq<voice_t, event_t>& voice, const event_meta_sq& event) mutable {
        if (logger && *logger) {
            gui_log_message msg;
            snprintf(
                    msg.text,
                    sizeof(msg.text),
                    " - %-16s %zu @ %f",
                    logging_name.c_str(),
                    event.pattern_index,
                    event.start_time);
            bool sent = (*logger)->gui.try_enqueue(msg);
            if (!sent) {
                std::fprintf(
                        stderr, "failed to log to gui: arrangement callback for %s\n", logging_name.c_str());
            }
        }
        voice.set_pattern(patterns[event.pattern_index]);
    };
}
