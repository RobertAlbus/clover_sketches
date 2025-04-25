#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <concepts>
#include <string>

template <typename T>
concept kick_drum_drawable_000 = requires(T kick, float a, float d, float s, float r) {
    { kick.props.to_str() } -> std::convertible_to<std::string>;

    { kick.adsr_amp.set(a, d, s, r) };
    { kick.adsr_pitch.set(a, d, s, r) };
    { kick.adsr_cut.set(a, d, s, r) };

    { kick.props.pitch_range } -> std::convertible_to<float>;
    { kick.props.drive } -> std::convertible_to<float>;
    { kick.props.trim } -> std::convertible_to<float>;
    { kick.props.cut_range } -> std::convertible_to<float>;

    { kick.props.amp_a } -> std::convertible_to<float>;
    { kick.props.amp_d } -> std::convertible_to<float>;
    { kick.props.amp_s } -> std::convertible_to<float>;
    { kick.props.amp_r } -> std::convertible_to<float>;

    { kick.props.pitch_a } -> std::convertible_to<float>;
    { kick.props.pitch_d } -> std::convertible_to<float>;
    { kick.props.pitch_s } -> std::convertible_to<float>;
    { kick.props.pitch_r } -> std::convertible_to<float>;

    { kick.props.cut_a } -> std::convertible_to<float>;
    { kick.props.cut_d } -> std::convertible_to<float>;
    { kick.props.cut_s } -> std::convertible_to<float>;
    { kick.props.cut_r } -> std::convertible_to<float>;
};
