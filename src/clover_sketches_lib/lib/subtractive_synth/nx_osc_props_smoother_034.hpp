#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/dsp/env_linear.hpp"

#include "lib/subtractive_synth/nx_osc_034.hpp"

using nx_osc_props_env_034 = nx_osc_props_034_t<clover::dsp::env_linear>;
nx_osc_props_env_034 build_from(const nx_osc_props_034& from);

template <typename Props, typename Range>
concept range_of = std::ranges::forward_range<Range> &&
                   std::convertible_to<std::remove_cvref_t<std::ranges::range_reference_t<Range>>&, Props&>;

struct nx_osc_props_smoother_034 {
    nx_osc_props_smoother_034(const nx_osc_props_034& control_props)
        : control_props{control_props}, env{build_from(control_props)} {
    }
    const nx_osc_props_034& control_props;
    nx_osc_props_env_034 env;
    float smooth_duration = 1000;

    template <typename Range>
        requires range_of<nx_osc_034, Range>
    void tick(Range&& voice_props);
};

/*
This is quite... something. But it works!
I do not want to implement this for every single type that needs it however.
I should look at how JUCE, ASPiK, etc. implement property smoothing.
*/
template <typename Range>
    requires range_of<nx_osc_034, Range>
void nx_osc_props_smoother_034::tick(Range&& voices) {
    using env_t = clover::dsp::env_linear;

    // auto& voices_view = voices;

    auto set_env_if_changed = [this](env_t& e, float v) {
        if (e.m_to != v)
            e.set(v, smooth_duration);
    };
    auto in_progress = [](env_t const& e) { return e.m_current_step <= e.m_target_step; };
    auto view_prop   = [&](auto ptr) {
        return voices | std::views::transform([ptr](nx_osc_034& v) -> float& { return ((v.props).*ptr); });
    };
    auto view_el = [&](auto ptr, size_t idx) {
        return voices |
               std::views::transform([ptr, idx](nx_osc_034& v) -> float& { return ((v.props).*ptr)[idx]; });
    };
    auto smooth_prop = [&](env_t& e, float prop_val, auto prop_ptr) {
        set_env_if_changed(e, prop_val);
        if (in_progress(e)) {
            for (float& r : view_prop(prop_ptr)) {
                r = e.tick();
            }
            return true;
        }
        return false;
    };
    auto smooth_vector_prop = [&](env_t& e, float val, auto ptr, size_t idx) {
        set_env_if_changed(e, val);
        if (in_progress(e)) {
            for (float& r : view_el(ptr, idx)) {
                r = e.tick();
            }
            return true;
        }
        return false;
    };

    smooth_prop(env.tuning, control_props.tuning, &nx_osc_props_034::tuning);
    smooth_prop(env.portamento_time, control_props.portamento_time, &nx_osc_props_034::portamento_time);
    smooth_prop(env.pitch_env_octaves, control_props.pitch_env_octaves, &nx_osc_props_034::pitch_env_octaves);
    env.retrigger = control_props.retrigger;

    bool amp_adsr_changed = false;
    amp_adsr_changed |= smooth_prop(env.amp_a, control_props.amp_a, &nx_osc_props_034::amp_a);
    amp_adsr_changed |= smooth_prop(env.amp_d, control_props.amp_d, &nx_osc_props_034::amp_d);
    amp_adsr_changed |= smooth_prop(env.amp_s, control_props.amp_s, &nx_osc_props_034::amp_s);
    amp_adsr_changed |= smooth_prop(env.amp_r, control_props.amp_r, &nx_osc_props_034::amp_r);

    if (amp_adsr_changed) {
        for (auto& voice : voices) {
            voice.adsr_amp.set(
                control_props.amp_a, control_props.amp_d, control_props.amp_s, control_props.amp_r);
        }
    }

    bool pitch_adsr_changed = false;
    pitch_adsr_changed |= smooth_prop(env.pitch_a, control_props.pitch_a, &nx_osc_props_034::pitch_a);
    pitch_adsr_changed |= smooth_prop(env.pitch_d, control_props.pitch_d, &nx_osc_props_034::pitch_d);
    pitch_adsr_changed |= smooth_prop(env.pitch_s, control_props.pitch_s, &nx_osc_props_034::pitch_s);
    pitch_adsr_changed |= smooth_prop(env.pitch_r, control_props.pitch_r, &nx_osc_props_034::pitch_r);

    if (pitch_adsr_changed) {
        for (auto& voice : voices) {
            voice.adsr_pitch.set(
                control_props.pitch_a, control_props.pitch_d, control_props.pitch_s, control_props.pitch_r);
        }
    }

    const size_t size = control_props.osc_tunings.size();
    for (auto i : std::views::iota(0u, size)) {
        smooth_vector_prop(
            env.osc_tunings[i], control_props.osc_tunings[i], &nx_osc_props_034::osc_tunings, i);
        if (smooth_vector_prop(env.osc_pans[i], control_props.osc_pans[i], &nx_osc_props_034::osc_pans, i)) {
            for (auto& voice : voices) {
                voice.osc_pans[i].set(control_props.osc_pans[i]);
            }
        }
        smooth_vector_prop(env.osc_gains[i], control_props.osc_gains[i], &nx_osc_props_034::osc_gains, i);
        if (env.waveforms[i] != control_props.waveforms[i]) {
            env.waveforms[i] = control_props.waveforms[i];
            for (auto& voice : voices) {
                voice.oscs[i].waveform = waveform_to_func(control_props.waveforms[i]);
            }
        }
    }
}
