#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "subtractive_synth_036.hpp"
#include <ranges>

void update_subtractive_synth(const subtractive_synth_props_036& target, std::ranges::range auto& voices) {
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

    for (auto i : std::views::iota(0, subtractive_synth_props_036::NUM_OSCS)) {
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

    if (first_synth.props.pitch_a != target.pitch_a || first_synth.props.pitch_d != target.pitch_d ||
        first_synth.props.pitch_s != target.pitch_s || first_synth.props.pitch_r != target.pitch_r) {
        for (auto& voice : voices) {
            voice.props.pitch_a = target.pitch_a;
            voice.props.pitch_d = target.pitch_d;
            voice.props.pitch_s = target.pitch_s;
            voice.props.pitch_r = target.pitch_r;
            voice.adsr_pitch.set(
                voice.props.pitch_a, voice.props.pitch_d, voice.props.pitch_s, voice.props.pitch_r);
        }
    }

    if (first_synth.props.amp_a != target.amp_a || first_synth.props.amp_d != target.amp_d ||
        first_synth.props.amp_s != target.amp_s || first_synth.props.amp_r != target.amp_r) {
        for (auto& voice : voices) {
            voice.props.amp_a = target.amp_a;
            voice.props.amp_d = target.amp_d;
            voice.props.amp_s = target.amp_s;
            voice.props.amp_r = target.amp_r;
            voice.adsr_amp.set(voice.props.amp_a, voice.props.amp_d, voice.props.amp_s, voice.props.amp_r);
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

    if (first_synth.props.cut_a != target.cut_a || first_synth.props.cut_d != target.cut_d ||
        first_synth.props.cut_s != target.cut_s || first_synth.props.cut_r != target.cut_r) {
        for (auto& voice : voices) {
            voice.props.cut_a = target.cut_a;
            voice.props.cut_d = target.cut_d;
            voice.props.cut_s = target.cut_s;
            voice.props.cut_r = target.cut_r;
            voice.adsr_cut.set(voice.props.cut_a, voice.props.cut_d, voice.props.cut_s, voice.props.cut_r);
        }
    }

    if (first_synth.props.res_a != target.res_a || first_synth.props.res_d != target.res_d ||
        first_synth.props.res_s != target.res_s || first_synth.props.res_r != target.res_r) {
        for (auto& voice : voices) {
            voice.props.res_a = target.res_a;
            voice.props.res_d = target.res_d;
            voice.props.res_s = target.res_s;
            voice.props.res_r = target.res_r;
            voice.adsr_res.set(voice.props.res_a, voice.props.res_d, voice.props.res_s, voice.props.res_r);
        }
    }
}

void update_subtractive_synth(const subtractive_synth_props_036& target, subtractive_synth_036& voice) {
    auto single_view = std::views::single(voice);
    update_subtractive_synth(target, single_view);
}
