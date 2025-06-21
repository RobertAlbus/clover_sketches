#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/cymbal/cymbal_024.hpp"
#include "lib/fdn/fdn8_023.hpp"
#include "lib/fdn/fdn8_stereo_030.hpp"
#include "lib/kick_drum/kick_drum.hpp"
#include "lib/mixer/mixer.hpp"
#include "lib/peq/peq.hpp"
#include "lib/subtractive_synth/subtractive_synth.hpp"

#include "patches/patches.hpp"

struct signal_graph {
    float fs;
    patch patch;

    std::vector<mixer_track_000> mixer_tracks;
    std::unordered_map<std::string, std::reference_wrapper<float>> audio_mixer;

    std::pair<float, float> tick();

    float gain_master = 0.5f;
    signal_graph(float fs);

    // --------------------------------
    // KICK

    kick_drum_000 kick{fs, patch.drums.kick_drum_props};
    peq_000 kick_preverb_peq{fs, patch.drums.kick_preverb_peq_props};
    fdn8_023 kick_verb{fs, patch.drums.kick_fdn_props};
    peq_000 kick_out_peq{fs, patch.drums.kick_peq_props};

    cymbal_024 ride{fs, patch.drums.ride_props};
    peq_000 ride_peq{fs, patch.drums.ride_peq_props};

    // --------------------------------
    // CHORD

    std::array<subtractive_synth_000, 6> chord{
            subtractive_synth_000{fs, patch.synth.chord_props},
            subtractive_synth_000{fs, patch.synth.chord_props},
            subtractive_synth_000{fs, patch.synth.chord_props},
            subtractive_synth_000{fs, patch.synth.chord_props},
            subtractive_synth_000{fs, patch.synth.chord_props},
            subtractive_synth_000{fs, patch.synth.chord_props}};

    peq_000 chord_preverb_peq{fs, patch.synth.chord_preverb_peq_props};
    fdn8_stereo_030 chord_verb{fs, patch.synth.chord_fdn_props};
    peq_000 chord_peq{fs, patch.synth.chord_peq_props};

    peq_000 main_eq{fs, patch.mix.main_peq_props};
};
