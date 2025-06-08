#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/peq/peq.hpp"

#include "filter_response.hpp"

/*
# first iteration
- assume unidirectional control.
- can enhance later with observer vs. controller versions
- can create a dsp eq unit with mod targets and ui controls!
*/

struct gpeq_ui_028 {
    const char* name;
    gpeq_ui_028(const char* name, size_t num_graph_points, peq_000& peq);

    std::array<complex_response, peq_000::SIZE> complex_responses;
    complex_response cumulative_complex_response;
    std::vector<float> freqs;
    std::vector<double> axis_ticks;
    computed_response computed;

    std::array<peq_props_000, peq_000::SIZE> gui_peq_props;
    peq_000& peq;

    void update();
    void update_all();

    bool draw();
    void draw_response();
    bool draw_controls();
};
