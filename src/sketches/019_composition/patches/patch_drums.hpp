#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "instruments/cymbal.hpp"
#include "instruments/fdn.hpp"
#include "instruments/kick.hpp"
#include "instruments/peq.hpp"
#include "instruments/subtractive_synth.hpp"

struct patch_drums {
    // --------------------------------
    // KICK

    static struct {
        float send = 1.f;
        float wet  = 1.f;
        float gain = 1.f;
    } const kick_mix;

    static constexpr kick_props kick_drum_props{};
    static constexpr fdn_8_props_012 kick_fdn_props{};

    static constexpr std::array<peq_props, peq::SIZE> kick_preverb_peq_props{};
    static constexpr std::array<peq_props, peq::SIZE> kick_peq_props{};

    // --------------------------------
    // BASS

    static constexpr subtractive_synth_props bass_props{};
    static constexpr std::array<peq_props, peq::SIZE> bass_peq_props{};

    static struct {
        float gain = 1.f;
    } const bass_mix;

    // --------------------------------
    // CYMBALS

    static constexpr cymbal_props hh1_props{};

    static constexpr cymbal_props hh2_props{};
    static constexpr subtractive_synth_props hh3_props{.osc_props{}, .filter_props{}};

    static constexpr fdn_8_props_012 hh_fdn_props{};
    static constexpr std::array<peq_props, peq::SIZE> hh_preverb_peq_props{};
    static constexpr std::array<peq_props, peq::SIZE> hh_peq_props{};

    static constexpr cymbal_props ride_props{};
    static constexpr std::array<peq_props, peq::SIZE> ride_peq_props{};

    static struct {
        float mix_hh1 = 1.f;
        float mix_hh2 = 1.f;
        float mix_hh3 = 1.f;

        float hh_verb_send = 1.f;
        float hh_verb_wet  = 1.f;
        float mix_hh       = 1.f;

        float mix_ride = 1.f;

        float mix_bus = 1.f;
    } const hh_mix;
};
