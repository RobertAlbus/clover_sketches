#pragma once

#include <complex>
#include <vector>

#include "clover/dsp/filter.hpp"

using complex_response = std::vector<std::complex<float>>;

struct computed_response {
    std::vector<float> magnitudes;
    std::vector<float> angles;
    computed_response(size_t size) : magnitudes(size, 0), angles(size, 0) {
    }
};

void log_spaced_freqs(std::vector<float>& out, float start_freq, float stop_freq);

void compute_complex_response(
    const clover::dsp::iir_coeffs& coeffs, complex_response& out, const std::vector<float>& freqs);

void compute_spectrum(const complex_response& spectrum, computed_response& out);

void unwrap_phase(std::vector<float>& angles);
