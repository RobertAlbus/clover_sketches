#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <string>

#include "clover/io/audio_callback.hpp"

struct behaviour_base {
    // render behaviours
    virtual ~behaviour_base()                    = default;
    virtual int render_duration_samples()        = 0;
    virtual int render_runout_duration_samples() = 0;
    virtual void sequencer_start()               = 0;
    virtual void sequencer_stop()                = 0;
    virtual constexpr std::string project_name() = 0;

    // live behaviours
    virtual bool view_draw()                             = 0;
    virtual void view_init()                             = 0;
    virtual void view_deinit()                           = 0;
    virtual clover::io::callback create_audio_callback() = 0;
    virtual int channel_count_out()                      = 0;
    virtual float fs()                                   = 0;
};
