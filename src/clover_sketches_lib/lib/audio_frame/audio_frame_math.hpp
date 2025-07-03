#pragma once
#include "audio_frame.hpp"
#include <algorithm>
#include <cmath>

inline constexpr audio_frame_stereo clamp(audio_frame_stereo x, float lo, float hi) {
    return {std::clamp(x.L, lo, hi), std::clamp(x.R, lo, hi)};
}

inline constexpr audio_frame_stereo clamp(
        audio_frame_stereo x, audio_frame_stereo lo, audio_frame_stereo hi) {
    return {std::clamp(x.L, lo.L, hi.L), std::clamp(x.R, lo.R, hi.R)};
}

#define DEFINE_UNARY_MATH(fn)                                      \
    inline constexpr audio_frame_stereo fn(audio_frame_stereo x) { \
        return {std::fn(x.L), std::fn(x.R)};                       \
    }

DEFINE_UNARY_MATH(sin)
DEFINE_UNARY_MATH(cos)
DEFINE_UNARY_MATH(tan)
DEFINE_UNARY_MATH(asin)
DEFINE_UNARY_MATH(acos)
DEFINE_UNARY_MATH(atan)
DEFINE_UNARY_MATH(sinh)
DEFINE_UNARY_MATH(cosh)
DEFINE_UNARY_MATH(tanh)
DEFINE_UNARY_MATH(exp)
DEFINE_UNARY_MATH(log)
DEFINE_UNARY_MATH(log10)
DEFINE_UNARY_MATH(sqrt)
DEFINE_UNARY_MATH(ceil)
DEFINE_UNARY_MATH(floor)
DEFINE_UNARY_MATH(abs)
#undef DEFINE_UNARY_MATH

inline constexpr audio_frame_stereo fmod(audio_frame_stereo a, audio_frame_stereo b) {
    return {std::fmod(a.L, b.L), std::fmod(a.R, b.R)};
}
inline constexpr audio_frame_stereo pow(audio_frame_stereo a, audio_frame_stereo b) {
    return {std::pow(a.L, b.L), std::pow(a.R, b.R)};
}

template <typename T>
    requires std::is_arithmetic_v<T>
inline constexpr audio_frame_stereo pow(audio_frame_stereo a, T b) {
    auto fb = static_cast<float>(b);
    return {std::pow(a.L, fb), std::pow(a.R, fb)};
}
template <typename T>
    requires std::is_arithmetic_v<T>
inline constexpr audio_frame_stereo pow(T a, audio_frame_stereo b) {
    auto fa = static_cast<float>(a);
    return {std::pow(fa, b.L), std::pow(fa, b.R)};
}
