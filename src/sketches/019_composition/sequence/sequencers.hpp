#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/cymbal/cymbal.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "lib/subtractive_synth/subtractive_synth.hpp"

#include "composition/composition.hpp"
#include "instruments/frsq.hpp"
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
    frsq_019<event, event> frsq_arrangement_print;

    // pattern sequencers
    frsq_019<kick_drum_000, event> frsq_kick;
    frsq_019<subtractive_synth, event_midi> frsq_bass;
    frsq_019<cymbal, event> frsq_hh1;
    frsq_019<cymbal, event> frsq_hh2;
    frsq_019<subtractive_synth, event> frsq_hh3;
    frsq_019<cymbal, event> frsq_ride;
    frsq_019<subtractive_synth, event_midi> frsq_chord;
    frsq_019<subtractive_synth, event_midi> frsq_lead_a1;
    frsq_019<subtractive_synth, event_midi> frsq_lead_a2;
    frsq_019<nx_osc, event_midi> frsq_lead_b1;
    frsq_019<nx_osc, event_midi> frsq_lead_b2;
    frsq_019<subtractive_synth, event_midi> frsq_pad;

    // meta sequencers
    frsq_019<frsq_019<kick_drum_000, event>, event_meta_sq> meta_frsq_kick;
    frsq_019<frsq_019<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_bass;
    frsq_019<frsq_019<cymbal, event>, event_meta_sq> meta_frsq_hh1;
    frsq_019<frsq_019<cymbal, event>, event_meta_sq> meta_frsq_hh2;
    frsq_019<frsq_019<subtractive_synth, event>, event_meta_sq> meta_frsq_hh3;
    frsq_019<frsq_019<cymbal, event>, event_meta_sq> meta_frsq_ride;
    frsq_019<frsq_019<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_chord;
    frsq_019<frsq_019<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_lead_a1;
    frsq_019<frsq_019<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_lead_a2;
    frsq_019<frsq_019<nx_osc, event_midi>, event_meta_sq> meta_frsq_lead_b1;
    frsq_019<frsq_019<nx_osc, event_midi>, event_meta_sq> meta_frsq_lead_b2;
    frsq_019<frsq_019<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_pad;

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