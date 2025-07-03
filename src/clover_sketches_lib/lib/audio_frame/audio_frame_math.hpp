#pragma once
#include "audio_frame.hpp"
#include <algorithm>
#include <cmath>

inline constexpr audio_frame clamp(audio_frame x, float lo, float hi) {
    return {std::clamp(x.L, lo, hi), std::clamp(x.R, lo, hi)};
}

inline constexpr audio_frame clamp(audio_frame x, audio_frame lo, audio_frame hi) {
    return {std::clamp(x.L, lo.L, hi.L), std::clamp(x.R, lo.R, hi.R)};
}

#define DEFINE_UNARY_MATH(fn)                        \
    inline constexpr audio_frame fn(audio_frame x) { \
        return {std::fn(x.L), std::fn(x.R)};         \
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

inline constexpr audio_frame fmod(audio_frame a, audio_frame b) {
    return {std::fmod(a.L, b.L), std::fmod(a.R, b.R)};
}
inline constexpr audio_frame pow(audio_frame a, audio_frame b) {
    return {std::pow(a.L, b.L), std::pow(a.R, b.R)};
}

template <typename T>
    requires std::is_arithmetic_v<T>
inline constexpr audio_frame pow(audio_frame a, T b) {
    auto fb = static_cast<float>(b);
    return {std::pow(a.L, fb), std::pow(a.R, fb)};
}
template <typename T>
    requires std::is_arithmetic_v<T>
inline constexpr audio_frame pow(T a, audio_frame b) {
    auto fa = static_cast<float>(a);
    return {std::pow(fa, b.L), std::pow(fa, b.R)};
}
