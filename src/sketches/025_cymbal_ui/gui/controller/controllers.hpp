#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/logging/logger.hpp"

#include "graph/graph.hpp"

struct tabbed_controller {
    const char* name;
    void (*controller)(const char*, signal_graph&, log_bus_000&);
};

void controller_mixer(const char* id, signal_graph& graph, log_bus_000& logger);
void controller_kick(const char* id, signal_graph& graph, log_bus_000& logger);
void controller_ride(const char* id, signal_graph& graph, log_bus_000& logger);
void controller_chord(const char* id, signal_graph& graph, log_bus_000& logger);
