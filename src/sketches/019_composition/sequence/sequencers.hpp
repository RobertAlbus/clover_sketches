#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/composition.hpp"
#include "instruments/cymbal.hpp"
#include "instruments/frsq.hpp"
#include "instruments/kick.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/event.hpp"
#include "sequence/pattern_drum.hpp"
#include "sequence/pattern_synth.hpp"

struct sequencers {
    pattern_drum drum_patterns;
    pattern_synth synth_patterns;

    frsq<kick_drum, event> frsq_kick;
    frsq<subtractive_synth, event_midi> frsq_bass;

    frsq<cymbal, event> frsq_hh1;
    frsq<cymbal, event> frsq_hh2;
    frsq<subtractive_synth, event> frsq_hh3;
    frsq<cymbal, event> frsq_ride;

    frsq<subtractive_synth, event_midi> frsq_chord;
    frsq<subtractive_synth, event_midi> frsq_lead_a1;
    frsq<subtractive_synth, event_midi> frsq_lead_a2;
    frsq<subtractive_synth, event_midi> frsq_lead_a3;
    frsq<subtractive_synth, event_midi> frsq_lead_b1;
    frsq<subtractive_synth, event_midi> frsq_lead_b2;
    frsq<subtractive_synth, event_midi> frsq_lead_b3;

    frsq<subtractive_synth, event_midi> frsq_pad;

    sequencers(composition& comp);
    void tick();

    void set_up_kick(composition& comp);
    void set_up_bass(composition& comp);

    void set_up_hh1(composition& comp);
    void set_up_hh2(composition& comp);
    void set_up_hh3(composition& comp);
    void set_up_ride(composition& comp);

    void set_up_chord(composition& comp);
    void set_up_pad(composition& comp);
    void set_up_lead_a(composition& comp);
};