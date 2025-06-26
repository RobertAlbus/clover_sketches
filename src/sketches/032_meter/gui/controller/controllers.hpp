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
    tabbed_controller(const char* name, sequencers& sqs, signal_graph& graph, log_bus_000& logger)
        : name{name}, sqs{sqs}, graph{graph}, logger{logger} {};
    virtual ~tabbed_controller() = default;

    const char* name;
    sequencers& sqs;
    signal_graph& graph;
    log_bus_000& logger;
    virtual void draw() = 0;
};

struct controller_mixer : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 master_peq{"##main_peq", graph.main_eq};

    void draw() override;
};

struct controller_kick : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 gpeq_send{"##kick_preverb_peq", graph.kick_preverb_peq};
    gpeq_ui_028 gpeq_post{"##kick_out_peq", graph.kick_out_peq};
    void draw() override;
};

struct controller_snare : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    void draw() override;
};

struct controller_ride : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 gpeq_cymbal{"##ride_peq", graph.ride_peq};
    void draw() override;
};

struct controller_chord : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 gpeq_send{"##chord_preverb_peq", graph.chord_preverb_peq};
    gpeq_ui_028 gpeq_post{"##chord_peq", graph.chord_peq};
    void draw() override;
};
