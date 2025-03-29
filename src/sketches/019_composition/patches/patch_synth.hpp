#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "instruments/fdn.hpp"
#include "instruments/nx_osc.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"

struct patch_synth_t {
    // --------------------------------
    // LEAD

    subtractive_synth_props lead_props{.osc_props{}, .filter_props{}};
    nx_osc_props lead_ringmod_props{};
    std::array<peq_props, peq::SIZE> lead_peq_props{};

    static struct {
        float carrier   = 1.f;
        float modulator = 1.f;
        float modulated = 1.f;
    } const lead_mix;

    // --------------------------------
    // CHORD

    subtractive_synth_props chord_props{.osc_props{}, .filter_props{}};

    fdn_8_props_012 chord_fdn_props{};
    std::array<peq_props, peq::SIZE> chord_preverb_peq_props{};
    std::array<peq_props, peq::SIZE> chord_peq_props{};

    // --------------------------------
    // PAD

    subtractive_synth_props pad_props{.osc_props{}, .filter_props{}};

    fdn_8_props_012 pad_fdn_props{};
    std::array<peq_props, peq::SIZE> pad_preverb_peq_props{};
    std::array<peq_props, peq::SIZE> pad_peq_props{};
};