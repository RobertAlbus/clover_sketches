// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <print>
#include <ranges>
#include <utility>

#include "clover/dsp/filter.hpp"

#include "peq.hpp"

std::string peq_props_000::to_str() {
    return std::format(
        R"(peq_props_000{{
        .freq = {},
        .reso = {},
        .gain = {},
        .enabled = {},
        .type = peq_filter_type::{},
    }})",
        freq,
        reso,
        gain,
        enabled,
        peq_filter_str[int(type)]);
}

void clear_filter_state(clover::dsp::filter_2& filter) {
    std::fill(filter.m_inputs.begin(), filter.m_inputs.end(), 0);
    std::fill(filter.m_outputs.begin(), filter.m_outputs.end(), 0);
}

bool nearly_equal(float a, float b) {
    float diff    = std::fabs(a - b);
    float largest = std::max(std::fabs(a), std::fabs(b));
    return diff <= std::numeric_limits<float>::epsilon() * std::max(1.0f, largest);
}

bool operator==(const peq_props_000& a, const peq_props_000& b) {
    bool is_equal =                      //
        a.type == b.type &&              //
        a.enabled == b.enabled &&        //
        nearly_equal(a.freq, b.freq) &&  //
        nearly_equal(a.reso, b.reso);
    bool has_gain =
        (a.type == peq_filter_type::ls ||  //
         a.type == peq_filter_type::hs ||  //
         a.type == peq_filter_type::eq     //
        );

    if (has_gain) {
        bool is_gain_equal = nearly_equal(a.gain, b.gain);
        return is_equal && is_gain_equal;
    }

    return is_equal;
}

bool operator!=(const peq_props_000& a, const peq_props_000& b) {
    return !(a == b);
}

peq_props_000 lerp(const peq_props_000& a, const peq_props_000& b, float lerp_amount) {
    if (a == b || lerp_amount == 0.0f)
        return a;
    if (lerp_amount == 1.0f)
        return b;

    return {
        .freq    = std::lerp(a.freq, b.freq, lerp_amount),
        .reso    = std::lerp(a.reso, b.reso, lerp_amount),
        .gain    = std::lerp(a.gain, b.gain, lerp_amount),
        .enabled = b.enabled,
        .type    = b.type};
}

void update_peq_from_gui(peq_gui_model& gui_model, peq_000& audio_model) {
    for (auto [i, gui_dirty, gui, audio] : std::views::zip(
             std::views::iota(0, int(peq_000::SIZE)), gui_model.dirty, gui_model.props, audio_model.props))
        if (gui_dirty) {
            if (gui != audio) {
                if (audio.type != gui.type || (gui.enabled && !audio.enabled)) {
                    clear_filter_state(audio_model.filters[i]);
                }
                audio = lerp(audio, gui, 0.001);
                audio_model.calculate_coefficients(i);
            } else {
                gui_dirty = false;
            }
        }
}

peq_000::peq_000(float fs, const std::array<peq_props_000, peq_000::SIZE>& new_props) : fs{fs} {
    patch(new_props);
}
void peq_000::patch(std::array<peq_props_000, peq_000::SIZE> new_props) {
    props = std::move(new_props);
    for (auto i : std::views::iota(0, int(peq_000::SIZE)))
        if (props[i].enabled) {
            calculate_coefficients(i);
            clear_filter_state(filters[i]);
        }
}
std::string peq_000::to_str() {
    std::string output = "{\n";

    for (auto& prop : props) {
        output += "    ";
        output += prop.to_str();
        output += ",\n";
    }
    output.pop_back();  // remove trailing comma for last element
    output += "\n};";

    return output;
}

std::pair<float, float> peq_000::tick(float in) {
    return tick({in, in});
}
std::pair<float, float> peq_000::tick(float in_L, float in_R) {
    return tick({in_L, in_R});
}
std::pair<float, float> peq_000::tick(std::pair<float, float> in) {
    std::pair<float, float> out{in};
    for (auto [filter, props] : std::views::zip(filters, props)) {
        if (props.enabled) {
            out = filter.tick(out.first, out.second);
        }
    }

    return out;
}

void peq_000::set(size_t i, float new_freq, float new_reso, float new_gain) {
    props[i].freq = new_freq;
    props[i].reso = new_reso;
    props[i].gain = new_gain;
    if (props[i].enabled) {
        calculate_coefficients(i);
    }
}

void peq_000::set(size_t i, const peq_props_000& new_props) {
    props[i] = new_props;
    if (props[i].enabled) {
        calculate_coefficients(i);
    }
}

void peq_000::freq(size_t i, float value) {
    if (nearly_equal(props[i].freq, value))
        return;
    props[i].freq = value;
    calculate_coefficients(i);
}

void peq_000::reso(size_t i, float value) {
    if (nearly_equal(props[i].reso, value))
        return;
    props[i].reso = value;
    calculate_coefficients(i);
}

void peq_000::gain(size_t i, float value) {
    if (nearly_equal(props[i].gain, value))
        return;
    props[i].gain = value;
    calculate_coefficients(i);
}

void peq_000::type(size_t i, peq_filter_type value) {
    if (props[i].type == value)
        return;
    props[i].type = value;
    if (props[i].enabled) {
        calculate_coefficients(i);
        clear_filter_state(filters[i]);
    }
}

void peq_000::enabled(size_t i, bool value) {
    if (props[i].enabled == value)
        return;
    props[i].enabled = value;
    if (props[i].enabled) {
        calculate_coefficients(i);
        clear_filter_state(filters[i]);
    }
}

void peq_000::calculate_coefficients(size_t i) {
    filters[i].m_coeffs = filter_func[int(props[i].type)](fs, props[i].freq, props[i].reso, props[i].gain);
}
