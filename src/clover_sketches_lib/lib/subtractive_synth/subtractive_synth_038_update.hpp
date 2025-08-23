#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth_038.hpp"
#include <ranges>

namespace {
// Helper function to compare adsr_values structs
inline bool adsr_values_equal(const adsr_values& a, const adsr_values& b) {
    return a.a == b.a && a.d == b.d && a.s == b.s && a.r == b.r;
}
}  // namespace

void update_subtractive_synth(const subtractive_synth_props_038& target, std::ranges::range auto& voices) {
    if (std::ranges::empty(voices))
        return;

    auto& first_synth = *std::ranges::begin(voices);

    if (first_synth.props.tuning != target.tuning) {
        for (auto& voice : voices) {
            voice.props.tuning = target.tuning;
        }
    }

    if (first_synth.props.portamento_time != target.portamento_time) {
        for (auto& voice : voices) {
            voice.props.portamento_time = target.portamento_time;
            voice.portamento.set(target.portamento_time);
        }
    }

    if (first_synth.props.pitch_env_octaves != target.pitch_env_octaves) {
        for (auto& voice : voices) {
            voice.props.pitch_env_octaves = target.pitch_env_octaves;
        }
    }

    if (first_synth.props.retrigger != target.retrigger) {
        for (auto& voice : voices) {
            voice.props.retrigger = target.retrigger;
        }
    }

    for (auto i : std::views::iota(0, subtractive_synth_props_038::NUM_OSCS)) {
        if (first_synth.props.osc_pans[i] != target.osc_pans[i]) {
            for (auto& voice : voices) {
                voice.props.osc_pans[i] = target.osc_pans[i];
                voice.osc_pans[i].set(target.osc_pans[i]);
            }
        }
        if (first_synth.props.waveforms[i] != target.waveforms[i]) {
            for (auto& voice : voices) {
                voice.props.waveforms[i] = target.waveforms[i];
                voice.oscs[i].waveform   = waveform_to_func(target.waveforms[i]);
            }
        }
        for (auto& voice : voices) {
            voice.props.osc_tunings[i] = target.osc_tunings[i];
            voice.props.osc_gains[i]   = target.osc_gains[i];
        }
    }

    // Compare adsr_values structs atomically
    if (!adsr_values_equal(first_synth.props.pitch_adsr, target.pitch_adsr)) {
        for (auto& voice : voices) {
            voice.props.pitch_adsr = target.pitch_adsr;
            voice.adsr_pitch.set(
                voice.props.pitch_adsr.a,
                voice.props.pitch_adsr.d,
                voice.props.pitch_adsr.s,
                voice.props.pitch_adsr.r);
        }
    }

    if (!adsr_values_equal(first_synth.props.amp_adsr, target.amp_adsr)) {
        for (auto& voice : voices) {
            voice.props.amp_adsr = target.amp_adsr;
            voice.adsr_amp.set(
                voice.props.amp_adsr.a,
                voice.props.amp_adsr.d,
                voice.props.amp_adsr.s,
                voice.props.amp_adsr.r);
        }
    }

    if (first_synth.props.cut != target.cut || first_synth.props.res != target.res ||
        first_synth.props.filter_type != target.filter_type) {
        for (auto& voice : voices) {
            voice.props.cut         = target.cut;
            voice.props.res         = target.res;
            voice.props.filter_type = target.filter_type;
            voice.filter.m_coeffs =
                filter_func[int(voice.props.filter_type)](voice.fs, voice.props.cut, voice.props.res, 0);
        }
    }

    if (first_synth.props.cut_env_octaves != target.cut_env_octaves) {
        for (auto& voice : voices) {
            voice.props.cut_env_octaves = target.cut_env_octaves;
        }
    }

    if (first_synth.props.res_env_octaves != target.res_env_octaves) {
        for (auto& voice : voices) {
            voice.props.res_env_octaves = target.res_env_octaves;
        }
    }

    if (!adsr_values_equal(first_synth.props.cut_adsr, target.cut_adsr)) {
        for (auto& voice : voices) {
            voice.props.cut_adsr = target.cut_adsr;
            voice.adsr_cut.set(
                voice.props.cut_adsr.a,
                voice.props.cut_adsr.d,
                voice.props.cut_adsr.s,
                voice.props.cut_adsr.r);
        }
    }

    if (!adsr_values_equal(first_synth.props.res_adsr, target.res_adsr)) {
        for (auto& voice : voices) {
            voice.props.res_adsr = target.res_adsr;
            voice.adsr_res.set(
                voice.props.res_adsr.a,
                voice.props.res_adsr.d,
                voice.props.res_adsr.s,
                voice.props.res_adsr.r);
        }
    }
}

void update_subtractive_synth(const subtractive_synth_props_038& target, subtractive_synth_038& voice) {
    auto single_view = std::views::single(std::ref(voice)) |
                       std::views::transform([](auto& w) -> subtractive_synth_038& { return w.get(); });

    update_subtractive_synth(target, single_view);
}
