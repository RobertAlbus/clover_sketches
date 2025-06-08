#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/logging/logger.hpp"
#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "sequence/sequencers.hpp"

// consider ctor with sqs & graph to simplify indiviual controller ctors
struct tabbed_controller {
    tabbed_controller(const char* name) : name{name} {};
    virtual ~tabbed_controller() = default;

    const char* name;
    // draw does not need arguments
    virtual void draw(const char* id, signal_graph& graph, log_bus_000& logger) = 0;
};

struct controller_mixer : public tabbed_controller {
    controller_mixer(const char* name, sequencers& sqs, peq_000& master_peq)
        : tabbed_controller{name}, sqs(sqs), master_peq("master eq", master_peq) {};

    gpeq_ui_028 master_peq;
    sequencers& sqs;

    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};

struct controller_kick : public tabbed_controller {
    controller_kick(const char* name, peq_000& send_eq, peq_000& post_eq)
        : tabbed_controller{name},
          gpeq_send{"kick send peq", send_eq},
          gpeq_post("kick post peq", post_eq) {};
    gpeq_ui_028 gpeq_send;
    gpeq_ui_028 gpeq_post;
    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};

struct controller_ride : public tabbed_controller {
    controller_ride(const char* name, peq_000& cymbal_peq)
        : tabbed_controller{name}, gpeq_cymbal("cymbal peq", cymbal_peq) {};

    gpeq_ui_028 gpeq_cymbal;
    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};

struct controller_chord : public tabbed_controller {
    controller_chord(const char* name, peq_000& send_eq, peq_000& post_eq)
        : tabbed_controller{name},
          gpeq_send{"chord send peq", send_eq},
          gpeq_post("chord post peq", post_eq) {};

    gpeq_ui_028 gpeq_send;
    gpeq_ui_028 gpeq_post;
    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};
