#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <utility>

struct audio_frame_stereo {
    float L = 0;
    float R = 0;

    constexpr audio_frame_stereo() = default;
    constexpr audio_frame_stereo(float mono) : audio_frame_stereo(mono, mono) {
    }
    constexpr audio_frame_stereo(std::pair<float, float> p) : audio_frame_stereo(p.first, p.second)  {
    }
    constexpr audio_frame_stereo(float l, float r) : L(l), R(r) {
    }

    constexpr explicit operator float() const {
        return mid();
    }
    constexpr operator std::pair<float, float>() const {
        return {L, R};
    }
    constexpr std::pair<float, float> to_pair() const {
        return static_cast<std::pair<float, float>>(*this);
    }

    constexpr float mid() const {
        return 0.5f * (L + R);
    }
    constexpr float side() const {
        return 0.5f * (L - R);
    }
    constexpr audio_frame_stereo mono() const {
        return {mid(), mid()};
    }

    constexpr audio_frame_stereo to_mid_side() const {
        return {mid(), side()};
    }

    constexpr audio_frame_stereo from_mid_side() const {
        float mid  = L;
        float side = R;
        return {mid + side, mid - side};
    }

    constexpr audio_frame_stereo swap() const {
        return {R, L};
    }

    inline constexpr audio_frame_stereo operator+(audio_frame_stereo other) const {
        return {L + other.L, R + other.R};
    }
    inline constexpr audio_frame_stereo operator-(audio_frame_stereo other) const {
        return {L - other.L, R - other.R};
    }
    inline constexpr audio_frame_stereo operator*(audio_frame_stereo other) const {
        return {L * other.L, R * other.R};
    }
    inline constexpr audio_frame_stereo operator/(audio_frame_stereo other) const {
        return {L / other.L, R / other.R};
    }

    inline constexpr audio_frame_stereo& operator+=(audio_frame_stereo other) {
        L += other.L;
        R += other.R;
        return *this;
    }
    inline constexpr audio_frame_stereo& operator-=(audio_frame_stereo other) {
        L -= other.L;
        R -= other.R;
        return *this;
    }
    inline constexpr audio_frame_stereo& operator*=(audio_frame_stereo other) {
        L *= other.L;
        R *= other.R;
        return *this;
    }
    inline constexpr audio_frame_stereo& operator/=(audio_frame_stereo other) {
        L /= other.L;
        R /= other.R;
        return *this;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo operator+(T scalar) const {
        auto s = static_cast<float>(scalar);
        return {L + s, R + s};
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo operator-(T scalar) const {
        auto s = static_cast<float>(scalar);
        return {L - s, R - s};
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo operator*(T scalar) const {
        auto s = static_cast<float>(scalar);
        return {L * s, R * s};
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo operator/(T scalar) const {
        auto s = static_cast<float>(scalar);
        return {L / s, R / s};
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo& operator+=(T scalar) {
        auto s = static_cast<float>(scalar);
        L += s;
        R += s;
        return *this;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo& operator-=(T scalar) {
        auto s = static_cast<float>(scalar);
        L -= s;
        R -= s;
        return *this;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo& operator*=(T scalar) {
        auto s = static_cast<float>(scalar);
        L *= s;
        R *= s;
        return *this;
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    inline constexpr audio_frame_stereo& operator/=(T scalar) {
        auto s = static_cast<float>(scalar);
        L /= s;
        R /= s;
        return *this;
    }
};

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr audio_frame_stereo operator+(T scalar, audio_frame_stereo frame) {
    return frame + scalar;
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr audio_frame_stereo operator-(T scalar, audio_frame_stereo frame) {
    auto s = static_cast<float>(scalar);
    return {s - frame.L, s - frame.R};
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr audio_frame_stereo operator*(T scalar, audio_frame_stereo frame) {
    return frame * scalar;
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr audio_frame_stereo operator/(T scalar, audio_frame_stereo frame) {
    auto s = static_cast<float>(scalar);
    return {s / frame.L, s / frame.R};
}
