// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/mixer/mixer.hpp"
#include "patches.hpp"

patch_mix_t::patch_mix_t() {
    // alignment sucks...
    // could re-order mixer_track for gain first
    // then format the float to have N decimals.
    mixer_tracks = mixer_list_000{
            {.name = "kick bus", .gain = 0.9},
            {.spacer = true},
            {.name = "kick dry", .gain = 0.63043475},
            {.name = "kick send", .gain = 0.69},
            {.name = "kick wet", .gain = 0.46739125},
            {.spacer = true},
            {.name = "chord bus", .gain = 0.34},
            {.name = "chord dry", .gain = 0.326087},
            {.name = "chord send", .gain = 0.423913},
            {.name = "chord wet", .gain = 1.2391305},
    };

    main_peq_props = {
            peq_props_000{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props_000{
                    .freq    = 10,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::hp,
            },
            peq_props_000{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
            peq_props_000{
                    .freq    = 20000,
                    .reso    = 0.707,
                    .gain    = 0,
                    .enabled = true,
                    .type    = peq_filter_type::lp,
            },
    };
}