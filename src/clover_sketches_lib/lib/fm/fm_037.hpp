#pragma once

#include <string>

#include "clover/dsp/env_adsr.hpp"
#include "clover/dsp/filter.hpp"
#include "clover/dsp/oscillator.hpp"
#include "clover/dsp/pan.hpp"

#include "lib/_atom/filter_type.hpp"
#include "lib/audio_frame/audio_frame.hpp"

enum struct fm_tuning_type {
    ratio,
    hz,
};
struct adsr_values {
    float a;
    float d;
    float s;
    float r;
};
struct fm_props_037 {
    std::array<float, 6> tunings;
    std::array<fm_tuning_type, 6> tuning_types;

    std::array<float, 36> mod_matrix;
    float matrix_octave_range = 3;

    std::array<adsr_values, 6> amp_adsrs;
    std::array<adsr_values, 6> pitch_adsrs;
    std::array<float, 6> pitch_env_mod_depths;

    std::array<float, 6> op_pans;
    std::array<float, 6> op_output_gains;

    filter_type filter_type;
    adsr_values cut_adsr;
    adsr_values res_adsr;
    float cut;
    float cut_mod_target;
    float res;
    float res_mod_target;
    bool retrigger;

    // WARNING: heap allocation
    std::string to_str();
    static fm_props_037 init();
};

struct fm_037 {
    fm_037(float fs, const fm_props_037& new_props);
    void patch(fm_props_037 new_props);

    std::array<clover::dsp::oscillator, 6> ops;
    std::array<clover::dsp::env_adsr, 6> amp_adsrs;
    std::array<clover::dsp::env_adsr, 6> pitch_adsrs;
    std::array<clover::dsp::pan, 6> pans;
    clover::dsp::env_adsr adsr_cut;
    clover::dsp::env_adsr adsr_res;
    clover::dsp::filter_2 filter;

    fm_props_037 props;

    float fs;
    float freq;

    void key_on(float midi_note);
    void key_off();
    void set_amp_adsr(size_t i, adsr_values values);
    void set_pitch_adsr(size_t i, adsr_values values);
    void set_cut_adsr(adsr_values values);
    void set_res_adsr(adsr_values values);
    void set_filter(filter_type new_type, float cut, float res);
    void set_tuning(size_t i, fm_tuning_type tuning_type, float tuning);
    audio_frame tick();
    float get_tuning(fm_tuning_type tuning_type, float tuning);
};
