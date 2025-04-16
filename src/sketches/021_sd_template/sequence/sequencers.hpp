#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/composition.hpp"
#include "instruments/frsq.hpp"
#include "instruments/kick.hpp"
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
    frsq<subtractive_synth, event_midi> frsq_chord;

    // meta sequencers
    frsq<frsq<kick_drum, event>, event_meta_sq> meta_frsq_kick;
    frsq<frsq<subtractive_synth, event_midi>, event_meta_sq> meta_frsq_chord;

    sequencers(composition& comp);
    void tick();

    void set_up_kick(composition& comp);
    void set_up_chord(composition& comp);
    void set_up_meta_sq(composition& comp);
    void set_up_arrangement_print(composition& comp);
};