#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/composition.hpp"
#include "instruments/cymbal.hpp"
#include "instruments/frsq.hpp"
#include "instruments/kick.hpp"
#include "instruments/nx_osc.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/event.hpp"
#include "sequence/pattern_drum.hpp"
#include "sequence/pattern_meta.hpp"
#include "sequence/pattern_synth.hpp"

struct sequencers {
    pattern_drum drum_patterns;
    pattern_synth synth_patterns;
    pattern_meta meta_patterns;

    // need a placeholder for the voices, so reusing the event
    // this is fine because there is no need for a voice - will just println in the callback.
    frsq<event, event> frsq_arrangement_print;

    // pattern sequencers
    frsq<kick_drum, event> frsq_kick;
    frsq<subtractive_synth, event_midi> frsq_bass;
    frsq<cymbal, event> frsq_hh1;
    frsq<cymbal, event> frsq_hh2;
    frsq<subtractive_synth, event> frsq_hh3;
    frsq<cymbal, event> frsq_ride;
    frsq<subtractive_synth, event_midi> frsq_chord;
    frsq<subtractive_synth, event_midi> frsq_lead_a1;
    frsq<subtractive_synth, event_midi> frsq_lead_a2;
    frsq<nx_osc, event_midi> frsq_lead_b1;
    frsq<nx_osc, event_midi> frsq_lead_b2;
    frsq<subtractive_synth, event_midi> frsq_pad;

    // meta sequencers
    frsq<frsq<kick_drum, event>, event_meta_sq> meta_frsq_kick;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_bass;
    frsq<frsq<cymbal, event>, event_meta_sq> meta_frsq_hh1;
    frsq<frsq<cymbal, event>, event_meta_sq> meta_frsq_hh2;
    frsq<frsq<subtractive_synth, event>, event_meta_sq> meta_frsq_hh3;
    frsq<frsq<cymbal, event>, event_meta_sq> meta_frsq_ride;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_chord;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_lead_a1;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_lead_a2;
    frsq<frsq<nx_osc, event_midi>, event_meta_sq> meta_frsq_lead_b1;
    frsq<frsq<nx_osc, event_midi>, event_meta_sq> meta_frsq_lead_b2;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_pad;

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
    void set_up_lead_b(composition& comp);
    void set_up_meta_sq(composition& comp);
    void set_up_arrangement_print(composition& comp);
};