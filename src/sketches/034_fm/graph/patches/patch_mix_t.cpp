// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patches.hpp"

patch_mix_t::patch_mix_t() {
    new_mixer_spec = mixer_033::list_specs{
            {.name = "main", .props = {.gain = 1, .pan = 0, .mono = false, .mute = false}},
            {.spacer = true},
            {.name = "kick bus", .props = {.gain = 0.5543479, .pan = 0, .mono = false, .mute = false}},
            {.spacer = true},
            {.name = "kick dry", .props = {.gain = 0.3369565, .pan = 0, .mono = false, .mute = false}},
            {.name = "kick send", .props = {.gain = 0.69, .pan = 0, .mono = false, .mute = false}},
            {.name = "kick wet", .props = {.gain = 0.9347826, .pan = 0, .mono = false, .mute = false}},
            {.spacer = true},
            {.name = "snare sum", .props = {.gain = 0.96739125, .pan = 0, .mono = false, .mute = false}},
            {.spacer = true},
            {.name = "snare impulse", .props = {.gain = 0, .pan = 0, .mono = false, .mute = false}},
            {.name  = "snare impulse send",
             .props = {.gain = 0.8913044, .pan = 0, .mono = false, .mute = false}},
            {.name = "snare body", .props = {.gain = 0.9130435, .pan = 0, .mono = false, .mute = false}},
            {.name = "snare verb", .props = {.gain = 0.25, .pan = 0, .mono = false, .mute = false}},
            {.spacer = true},
            {.name = "ride", .props = {.gain = 0.19565213, .pan = 0, .mono = false, .mute = false}},
            {.spacer = true},
            {.name = "chord bus", .props = {.gain = 0.09782612, .pan = 0, .mono = false, .mute = false}},
            {.name = "chord dry", .props = {.gain = 0.13043475, .pan = 0, .mono = false, .mute = false}},
            {.name = "chord send", .props = {.gain = 0.5, .pan = 0, .mono = false, .mute = false}},
            {.name = "chord wet", .props = {.gain = 1.75, .pan = 0, .mono = false, .mute = false}},
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
                    .enabled = false,
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
