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
    std::string project_name;
    std::vector<std::unique_ptr<tabbed_controller>> tabs;
    sequencers& sqs;
    signal_graph& graph;
    log_bus_000& logger;
    bool show_log_canvas = false;
    bool use_dark_theme  = true;
    log_canvas_000 canvas;
    transport_ui_028 transport;

    view(std::string project_name, sequencers& sqs, signal_graph& graph, log_bus_000& logger);
    std::vector<std::unique_ptr<tabbed_controller>> create_tabs();

    // return: should_continue
    bool draw();
};
