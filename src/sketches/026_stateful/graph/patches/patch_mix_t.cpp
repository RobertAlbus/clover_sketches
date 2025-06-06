// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/mixer/mixer.hpp"
#include "patches.hpp"

patch_mix_t::patch_mix_t() {
    mixer_tracks = mixer_list_000{
            {.name = "kick bus", .gain = 0.9},
            {.spacer = true},
            {.name = "kick dry", .gain = 0.3369565},
            {.name = "kick send", .gain = 0.69},
            {.name = "kick wet", .gain = 0.2413044},
            {.spacer = true},
            {.name = "ride", .gain = 0.15391314},
            {.spacer = true},
            {.name = "chord bus", .gain = 0.34},
            {.name = "chord dry", .gain = 0.13043475},
            {.name = "chord send", .gain = 0.7608696},
            {.name = "chord wet", .gain = 1.1195652},
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
                    .freq    = 10000,
                    .reso    = 2.707,
                    .gain    = 6,
                    .enabled = true,
                    .type    = peq_filter_type::eq,
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
