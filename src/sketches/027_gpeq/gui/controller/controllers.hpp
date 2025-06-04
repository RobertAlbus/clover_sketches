#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "gui/components/gpeq.hpp"
#include "lib/logging/logger.hpp"

#include "graph/graph.hpp"

struct tabbed_controller {
    tabbed_controller(const char* name) : name{name} {};
    virtual ~tabbed_controller() = default;

    const char* name;
    virtual void draw(const char* id, signal_graph& graph, log_bus_000& logger) = 0;
};

struct controller_mixer : public tabbed_controller {
    controller_mixer(const char* name, peq_000& master_peq)
        : tabbed_controller{name}, master_peq("master eq", 1000, master_peq) {};

    gpeq_ui master_peq;

    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};

struct controller_kick : public tabbed_controller {
    using tabbed_controller::tabbed_controller;
    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};

struct controller_ride : public tabbed_controller {
    using tabbed_controller::tabbed_controller;
    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};

struct controller_chord : public tabbed_controller {
    using tabbed_controller::tabbed_controller;
    void draw(const char* id, signal_graph& graph, log_bus_000& logger) override;
};
