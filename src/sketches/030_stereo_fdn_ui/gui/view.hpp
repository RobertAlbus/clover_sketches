#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <memory>
#include <vector>

#include "lib/logging/log_canvas.hpp"
#include "lib/logging/logger.hpp"
#include "lib/transport/transport_ui_028.hpp"

#include "controller/controllers.hpp"
#include "graph/graph.hpp"
#include "sequence/sequencers.hpp"

struct view {
    std::vector<std::unique_ptr<tabbed_controller>> tabs;
    sequencers& sqs;
    signal_graph& graph;
    log_bus_000& logger;
    bool show_log_canvas = false;
    log_canvas_000 canvas;
    transport_ui_028 transport;

    view(sequencers& sqs, signal_graph& graph, log_bus_000& logger);
    std::vector<std::unique_ptr<tabbed_controller>> create_tabs();
    bool draw();
};
