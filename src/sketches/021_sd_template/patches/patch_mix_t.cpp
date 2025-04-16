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

            .chord_send = 0.7282609,
            .chord_wet  = 0.4778261,
            .chord_dry  = 0.32826102,
            .chord_sum  = 0.3369565,

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
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
    };
}