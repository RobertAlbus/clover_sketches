#include <cmath>
#include <complex>
#include <numbers>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "clover/dsp/filter.hpp"

#include "filter_response.hpp"

void log_spaced_freqs(std::vector<float>& out, float start_freq, float stop_freq) {
    float log_start_freq = std::log2(start_freq);
    float log_stop_freq  = std::log2(stop_freq);
    float log_span       = log_stop_freq - log_start_freq;

    const size_t num_points = out.size() - 1;
    for (size_t i = 0; i <= num_points; ++i) {
        const float log2_f = log_start_freq + ((float(i) / float(num_points)) * log_span);
        out[i]             = std::exp2(log2_f);
    }
}

void compute_complex_response(
        const clover::dsp::iir_coeffs& coeffs, complex_response& out, const std::vector<float>& freqs) {
    if (out.size() != freqs.size())
        throw std::invalid_argument(std::format(
                //
                "compute_response(coeffs, out, freqs): \n"
                "out.size() != freqs.size()\n"
                "{} {}",
                out.size(),
                freqs.size()));

    const float stop_freq = *(freqs.end() - 1);
    for (auto [freq, output, i] : std::views::zip(freqs, out, std::views::iota(0, (int)out.size()))) {
        const float omega           = 2.0f * std::numbers::pi_v<float> * freq / (2.0f * stop_freq);
        const std::complex<float> z = std::polar(1.0f, -omega);

        const std::complex<float> numerator   = coeffs.b0 + coeffs.b1 * z + coeffs.b2 * z * z;
        const std::complex<float> denominator = 1.0f + coeffs.a1 * z + coeffs.a2 * z * z;

        output = numerator / denominator;
    }
}

void compute_spectrum(const complex_response& spectrum, computed_response& out) {
    if (spectrum.size() != out.magnitudes.size() || spectrum.size() != out.angles.size())
        throw std::invalid_argument(std::format(
                "compute_spectrum(...) size mismatch {} {} {}",
                spectrum.size(),
                out.magnitudes.size(),
                out.angles.size()));

    for (auto [z, magnitude, angle] : std::views::zip(spectrum, out.magnitudes, out.angles)) {
        magnitude = std::abs(z);
        angle     = std::atan2(z.imag(), z.real());
    }
}

void unwrap_phase(std::vector<float>& angles) {
    float previous = angles[0];
    size_t size    = angles.size();
    for (size_t i = 1; i < size; ++i) {
        float& angle = angles[i];
        float delta  = previous - angle;

        float threshhold = 1.5f * std::numbers::pi_v<float>;
        if (std::abs(delta) > threshhold) {
            float direction = delta < 0 ? -1 : 1;
            for (size_t j = i; j < size; ++j) {
                angles[j] += (direction * 2.f * std::numbers::pi_v<float>);
            }
        }
        previous = angle;
    }
}
