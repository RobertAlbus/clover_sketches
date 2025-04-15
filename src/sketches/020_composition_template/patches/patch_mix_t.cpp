// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/mix.hpp"
#include "patches.hpp"

patch_mix_t::patch_mix_t() {
    mix = {
            .kick_send = 0.6904762,
            .kick_wet  = 0.5952381,
            .kick_gain = 0.90683234,

            .bass_gain = 0.5837476,

            .hh1 = 0.71739125,
            .hh2 = 0.6413044,
            .hh3 = 0.11956525,

            .hh_verb_send = 0.81107664,
            .hh_verb_wet  = 0.53260875,
            .hh_bus       = 0.8369565,

            .hh_drive = 1.5238096,
            .hh_trim  = 0.7619047,

            .ride = 0.13012405,

            .cymbal_bus = 0.5947206,

            .chord_send = 0.7282609,
            .chord_wet  = 0.4778261,
            .chord_dry  = 0.32826102,
            .chord_sum  = 0.3369565,

            .pad_send = 0.8369565,
            .pad_wet  = 0.923913,
            .pad_dry  = 0.7717391,
            .pad_sum  = 0.45347822,

            .lead_a       = 0.28571427,
            .lead_b       = 0,
            .lead_ringmod = 0.032608747,
            .lead         = 0.7608696,
    };

    main_peq_props = {
            peq_props{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 78.4,
                    .reso    = 0.9,
                    .gain    = -2.8,
                    .enabled = true,
                    .type    = peq_filter_type::ls,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = false,
                    .type    = peq_filter_type::lp,
            },
    };
}