#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "graph/graph.hpp"
#include <memory>
#include <vector>

#include "controller/controllers.hpp"
#include "lib/logging/logger.hpp"
#include "sequence/sequencers.hpp"

struct view {
    std::vector<std::unique_ptr<tabbed_controller>> tabs;
    sequencers& sqs;
    signal_graph& graph;
    log_bus_000& logger;

    view(sequencers& sqs, signal_graph& graph, log_bus_000& logger);
    std::vector<std::unique_ptr<tabbed_controller>> create_tabs();
    bool draw();
};
