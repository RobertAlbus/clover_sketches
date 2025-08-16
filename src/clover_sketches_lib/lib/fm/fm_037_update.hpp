#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "fm_037.hpp"
#include <ranges>

void update_fm_037(const fm_props_037& target, std::ranges::range auto& voices) {
    if (std::ranges::empty(voices))
        return;

    auto& first_fm = *std::ranges::begin(voices);

    for (auto i : std::views::iota(0, 6)) {
        if (first_fm.props.tunings[i] != target.tunings[i] ||
            first_fm.props.tuning_types[i] != target.tuning_types[i]) {
            for (auto& voice : voices) {
                voice.set_tuning(i, target.tuning_types[i], target.tunings[i]);
            }
        }
    }

    for (auto i : std::views::iota(0, 36)) {
        if (first_fm.props.mod_matrix[i] != target.mod_matrix[i]) {
            for (auto& voice : voices) {
                voice.props.mod_matrix[i] = target.mod_matrix[i];
            }
        }
    }

    if (first_fm.props.matrix_octave_range != target.matrix_octave_range) {
        for (auto& voice : voices) {
            voice.props.matrix_octave_range = target.matrix_octave_range;
        }
    }

    for (auto i : std::views::iota(0, 6)) {
        if (first_fm.props.amp_adsrs[i].a != target.amp_adsrs[i].a ||
            first_fm.props.amp_adsrs[i].d != target.amp_adsrs[i].d ||
            first_fm.props.amp_adsrs[i].s != target.amp_adsrs[i].s ||
            first_fm.props.amp_adsrs[i].r != target.amp_adsrs[i].r) {
            for (auto& voice : voices) {
                voice.props.amp_adsrs[i] = target.amp_adsrs[i];
                voice.set_amp_adsr(i, target.amp_adsrs[i]);
            }
        }
    }

    for (auto i : std::views::iota(0, 6)) {
        if (first_fm.props.pitch_adsrs[i].a != target.pitch_adsrs[i].a ||
            first_fm.props.pitch_adsrs[i].d != target.pitch_adsrs[i].d ||
            first_fm.props.pitch_adsrs[i].s != target.pitch_adsrs[i].s ||
            first_fm.props.pitch_adsrs[i].r != target.pitch_adsrs[i].r) {
            for (auto& voice : voices) {
                voice.props.pitch_adsrs[i] = target.pitch_adsrs[i];
                voice.set_pitch_adsr(i, target.pitch_adsrs[i]);
            }
        }
        for (auto& voice : voices) {
            voice.props.pitch_env_mod_depths[i] = target.pitch_env_mod_depths[i];
        }
    }

    for (auto i : std::views::iota(0, 6)) {
        if (first_fm.props.op_pans[i] != target.op_pans[i]) {
            for (auto& voice : voices) {
                voice.props.op_pans[i] = target.op_pans[i];
                voice.pans[i].set(target.op_pans[i]);
            }
        }
    }

    for (auto& voice : voices) {
        voice.props.op_output_gains = target.op_output_gains;
    }

    if (first_fm.props.filter_type != target.filter_type || first_fm.props.cut != target.cut ||
        first_fm.props.res != target.res) {
        for (auto& voice : voices) {
            voice.props.filter_type = target.filter_type;
            voice.props.cut         = target.cut;
            voice.props.res         = target.res;
            voice.set_filter(target.filter_type, target.cut, target.res);
        }
    }

    if (first_fm.props.cut_adsr.a != target.cut_adsr.a || first_fm.props.cut_adsr.d != target.cut_adsr.d ||
        first_fm.props.cut_adsr.s != target.cut_adsr.s || first_fm.props.cut_adsr.r != target.cut_adsr.r) {
        for (auto& voice : voices) {
            voice.props.cut_adsr = target.cut_adsr;
            voice.set_cut_adsr(target.cut_adsr);
        }
    }

    if (first_fm.props.res_adsr.a != target.res_adsr.a || first_fm.props.res_adsr.d != target.res_adsr.d ||
        first_fm.props.res_adsr.s != target.res_adsr.s || first_fm.props.res_adsr.r != target.res_adsr.r) {
        for (auto& voice : voices) {
            voice.props.res_adsr = target.res_adsr;
            voice.set_res_adsr(target.res_adsr);
        }
    }

    for (auto& voice : voices) {
        voice.props.cut_mod_target = target.cut_mod_target;
    }

    for (auto& voice : voices) {
        voice.props.res_mod_target = target.res_mod_target;
    }

    for (auto& voice : voices) {
        voice.props.retrigger = target.retrigger;
    }
}

void update_fm_037(const fm_props_037& target, fm_037& voice) {
    auto vr = std::views::counted(&voice, 1);
    update_fm_037(target, vr);
}
