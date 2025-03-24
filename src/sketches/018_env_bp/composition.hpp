#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>

#include "clover/io/audio_callback.hpp"

#include "composition/instruments/cymbal.hpp"
#include "composition/instruments/env_bp.hpp"
#include "composition/instruments/fdn.hpp"
#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/kick.hpp"
#include "composition/instruments/nx_osc.hpp"
#include "composition/instruments/peq.hpp"
#include "composition/patches.hpp"
#include "composition/patterns/sequencers.hpp"

using namespace clover;
using namespace io;

struct composition {
    std::pair<clover_float, clover_float> tick();

    float fs              = 48000;
    int fs_i              = static_cast<int>(fs);
    int channel_count_out = 2;
    double bpm            = 132;
    double spm            = fs * 60;
    double spb            = spm / bpm;
    double bar            = spb * 4;
    int_fast64_t duration = int_fast64_t(bar * 16 * 1000) + 1;

    kick_drum kick{fs, patch_deep_kick};
    cymbal hh1{fs};
    cymbal hh2{fs};
    env_bp hh2_articulation{pattern::hh2_articulation_env_pattern};

    std::array<nx_osc, 4> chords{
            nx_osc{fs, patch_deep_chord},
            nx_osc{fs, patch_deep_chord},
            nx_osc{fs, patch_deep_chord},
            nx_osc{fs, patch_deep_chord},
    };
    std::array<filter_block, 4> chord_filters{
            filter_block{fs, patch_deep_chord_filter},
            filter_block{fs, patch_deep_chord_filter},
            filter_block{fs, patch_deep_chord_filter},
            filter_block{fs, patch_deep_chord_filter},
    };

    std::array<subtractive_synth, 4> drones{fs, fs, fs, fs};

    fdn_8_012 kick_fdn_L{fs, patch_deep_fdn};
    fdn_8_012 kick_fdn_R{fs, patch_deep_fdn};

    float kick_mix     = 0.842;
    float hh1_mix      = 0.3;
    float hh2_mix      = 0.08;
    float reverb_mix   = 1;
    float verb_in_gain = 1;
    float chords_mix   = 0.08;
    float mix_beep     = 0.015;

    sequencers stsqs{fs, bpm, kick, hh1, hh2, hh2_articulation, chords, chord_filters, drones};

    peq kick_peq{fs, kick_peq_props};
    peq_gui_model kick_peq_gui{kick_peq_props};

    composition() {
        // shame on me. need to extract cymbal synth internals to props.
        for (auto &f : hh2.osc_freq)
            f = (f * 1.7f) + 390.f;
        hh2.amp_a = 10;
        hh2.amp_d = 800;
        hh2.cut_a = 10;
        hh2.cut_d = 800;
        hh2.set_oscs();
        hh2.set_adsrs();

        hh2_articulation.duration_abs = (bar / 16) * 5.;
        hh2_articulation.duration_rel = 5;
    }

    io::callback audio_callback = [&](callback_args data) {
        float &L = *(data.output);
        float &R = *(data.output + 1);

        std::tie(L, R) = tick();

        if (data.clock_time == duration) {
            return callback_status::end;
        }
        return callback_status::cont;
    };
};
