// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patches.hpp"

patch_mix_t::patch_mix_t() {
    new_mixer_spec = mixer_033::list_specs{
        {.name = "main", .props = { .gain = 1.4565217, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "kick bus", .props = { .gain = 0.78260875, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "kick dry", .props = { .gain = 0.61956525, .pan = 0, .mono = false, .mute = false}},
        {.name = "kick send", .props = { .gain = 1.3586956, .pan = 0, .mono = false, .mute = false}},
        {.name = "kick wet", .props = { .gain = 0.22826087, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "bass", .props = { .gain = 0.9021739, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "snare sum", .props = { .gain = 1.4673913, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "snare impulse send", .props = { .gain = 1.8695652, .pan = 0, .mono = false, .mute = false}},
        {.name = "snare body", .props = { .gain = 0.8478261, .pan = 0, .mono = false, .mute = false}},
        {.name = "snare noise", .props = { .gain = 1.2391305, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "cymbal sum", .props = { .gain = 0.9130435, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "hh1", .props = { .gain = 0.0869565, .pan = 0.27999997, .mono = false, .mute = false}},
        {.name = "hh2", .props = { .gain = 0.8043479, .pan = -0.20799997, .mono = false, .mute = false}},
        {.name = "hh3", .props = { .gain = 0.63043475, .pan = 0.120000005, .mono = false, .mute = false}},
        {.name = "ride", .props = { .gain = 0.6630435, .pan = -0.080000006, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "chord bus", .props = { .gain = 0.9130435, .pan = 0, .mono = false, .mute = false}},
        {.spacer = true},
        {.name = "chord dry", .props = { .gain = 0.5108696, .pan = 0, .mono = false, .mute = false}},
        {.name = "chord verb", .props = { .gain = 0.7608696, .pan = 0, .mono = false, .mute = false}},
    };

    main_peq_props = {
        peq_props_000{
            .freq    = 10,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::hp,
        },
        peq_props_000{
            .freq    = 10000,
            .reso    = 2.707,
            .gain    = 6,
            .enabled = false,
            .type    = filter_type::eq,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
        peq_props_000{
            .freq    = 20000,
            .reso    = 0.707,
            .gain    = 0,
            .enabled = true,
            .type    = filter_type::lp,
        },
    };
}
