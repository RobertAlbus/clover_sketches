#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/peq/gpeq.hpp"

#include "graph/graph.hpp"
#include "sequence/sequencers.hpp"

struct tabbed_controller {
    tabbed_controller(const char* name, sequencers& sqs, signal_graph& graph)
        : name{name}, sqs{sqs}, graph{graph} {};
    virtual ~tabbed_controller() = default;

    const char* name;
    sequencers& sqs;
    signal_graph& graph;
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

struct controller_bass : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    void draw() override;
};

struct controller_snare : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 gpeq{"##snare_eq", graph.snare_body_eq};
    gpeq_ui_028 gpeq_noise{"##snare_noise_eq", graph.snare_noise_eq};

    void draw() override;
};

struct controller_ride : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 gpeq_cymbal{"##ride_peq", graph.ride_peq};
    gpeq_ui_028 gpeq_hh1{"##hh1_peq", graph.hh1_peq};
    gpeq_ui_028 gpeq_hh2{"##hh2_peq", graph.hh2_peq};
    void draw() override;
};

struct controller_chord : public tabbed_controller {
    using tabbed_controller::tabbed_controller;

    gpeq_ui_028 gpeq_send{"##chord_peq", graph.chord_peq};
    void draw() override;
};
