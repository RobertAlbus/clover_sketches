#include <array>
#include <format>
#include <ranges>

#include "clover/dsp/wave.hpp"
#include "clover/math.hpp"
#include "lib/_atom/filter_type.hpp"

#include "fm_037.hpp"

template <size_t N>
std::string array_to_str(std::array<float, N> arr) {
    std::string result = "{";
    for (auto& value : arr)
        result += std::format("{}, ", std::to_string(value));
    result.pop_back();
    result += "}";
    return result;
}

std::string tuning_types_to_str(std::array<fm_tuning_type, 6> tuning_types) {
    std::string result = "{";
    for (auto& tuning_type : tuning_types) {
        switch (tuning_type) {
            case fm_tuning_type::ratio:
                result += "fm_tuning_type::ratio";
            case fm_tuning_type::hz:
                result += "fm_tuning_type::hz";
            default:
                result += "fm_tuning_type::ratio";
        }
        result += ", ";
    }
    result.pop_back();
    result += "}";
    return result;
}

std::string adsr_to_str(adsr_values adsr) {
    return std::format("{{ {}, {}, {}, {} }}", adsr.a, adsr.d, adsr.s, adsr.r);
}

std::string adsrs_to_str(std::array<adsr_values, 6> adsrs) {
    std::string result = "{";
    for (auto& adsr : adsrs) {
        result += adsr_to_str(adsr);
        result += ", ";
    }
    result.pop_back();
    result += "}";
    return result;
}

std::string fm_props_037::to_str() {
    return std::format(
        R"(
        {{
            .tunings = {},
            .tuning_types = {},
            .mod_matrix = {},
            .matrix_octave_range = {},
            .amp_adsrs = {},
            .pitch_adsrs = {},
            .pitch_env_mod_depths = {},
            .op_pans = {},
            .op_output_gains = {},
            .filter_type = filter_type::{},
            .cut_adsr = {}
            .res_adsr = {}
            .cut = {},
            .cut_mod_target = {},
            .res = {},
            .res_mod_target = {},
            .retrigger = {},
        }};)",

        array_to_str(tunings),
        tuning_types_to_str(tuning_types),
        array_to_str(mod_matrix),
        matrix_octave_range,
        adsrs_to_str(amp_adsrs),
        adsrs_to_str(pitch_adsrs),
        array_to_str(pitch_env_mod_depths),
        array_to_str(op_pans),
        array_to_str(op_output_gains),
        filter_str[int(filter_type)],
        adsr_to_str(cut_adsr),
        adsr_to_str(res_adsr),
        cut,
        cut_mod_target,
        res,
        res_mod_target,
        retrigger);
}

fm_props_037 fm_props_037::init() {
    adsr_values default_adsr = {100, 100, 1, 100};
    fm_props_037 props{};

    props.matrix_octave_range = 3.f;
    props.tunings.fill(1);
    props.tuning_types.fill(fm_tuning_type::ratio);
    props.amp_adsrs.fill(default_adsr);
    props.pitch_adsrs.fill(default_adsr);
    props.filter_type = filter_type::lp;
    props.cut_adsr    = default_adsr;
    props.res_adsr    = default_adsr;
    props.cut = props.cut_mod_target = 20000.f;
    props.res = props.res_mod_target = 0.707f;
    props.op_output_gains[0]         = 1.f;
    props.retrigger                  = true;

    return props;
}

fm_037::fm_037(float fs, const fm_props_037& new_props) : fs(fs), ops{fs, fs, fs, fs, fs, fs} {
    patch(new_props);
}

void fm_037::patch(fm_props_037 new_props) {
    props = new_props;

    for (auto [op, tuning, tuning_type] : std::views::zip(ops, props.tunings, props.tuning_types)) {
        op.freq(get_tuning(tuning_type, tuning));
        op.waveform = clover::dsp::wave_sine;
        op.phase(0);
    }
    for (auto [amp_adsr, amp_adsr_values, i] :
         std::views::zip(amp_adsrs, props.amp_adsrs, std::views::iota(0, 6))) {
        set_amp_adsr(i, amp_adsr_values);
    }
    for (auto [pitch_adsr, pitch_adsr_values, i] :
         std::views::zip(pitch_adsrs, props.pitch_adsrs, std::views::iota(0, 6))) {
        set_pitch_adsr(i, pitch_adsr_values);
    }
    for (auto [op_pan, op_pan_value] : std::views::zip(pans, props.op_pans)) {
        op_pan.set(op_pan_value);
    }

    set_cut_adsr(props.cut_adsr);
    set_res_adsr(props.res_adsr);
    set_filter(props.filter_type, props.cut, props.res);
}

void fm_037::key_on(float midi_note) {
    if (props.retrigger)
        for (auto& op : ops)
            op.phase(0);
    for (auto& adsr : amp_adsrs)
        adsr.key_on();
    for (auto& adsr : pitch_adsrs)
        adsr.key_on();
    adsr_cut.key_on();
    adsr_res.key_on();

    freq = clover::midi_to_frequency(midi_note);
}

void fm_037::key_off() {
    for (auto& adsr : amp_adsrs)
        adsr.key_off();
    for (auto& adsr : pitch_adsrs)
        adsr.key_off();
    adsr_cut.key_off();
    adsr_res.key_off();
}

void fm_037::set_amp_adsr(size_t i, adsr_values values) {
    amp_adsrs[i].set(values.a, values.d, values.s, values.r);
}
void fm_037::set_pitch_adsr(size_t i, adsr_values values) {
    pitch_adsrs[i].set(values.a, values.d, values.s, values.r);
}
void fm_037::set_cut_adsr(adsr_values values) {
    adsr_cut.set(values.a, values.d, values.s, values.r);
}
void fm_037::set_res_adsr(adsr_values values) {
    adsr_res.set(values.a, values.d, values.s, values.r);
}
void fm_037::set_filter(filter_type new_type, float cut, float res) {
    props.filter_type = new_type;
    props.cut         = cut;
    props.res         = res;
    filter.m_coeffs   = filter_func[int(new_type)](fs, props.cut, props.res, 0);
}

audio_frame fm_037::tick() {
    // get op signals
    std::array<float, 6> op_signals{0, 0, 0, 0, 0, 0};
    for (auto [op_signal, op, amp_adsr] : std::views::zip(op_signals, ops, amp_adsrs)) {
        op_signal = op.tick() * amp_adsr.tick();
    }

    // process matrix
    for (auto op_index : std::views::iota(0, 6)) {
        auto mod_matrix_row = props.mod_matrix | std::views::drop(op_index * 6) | std::views::take(6);

        float fm_amount               = 0;
        constexpr float fb_avoid_zone = 0.0001;
        for (auto [mod_amount, op_signal, i] :
             std::views::zip(mod_matrix_row, op_signals, std::views::iota(0, 6))) {
            float mod = mod_amount;
            if (i > op_index) {
                if (std::abs(mod_amount) < fb_avoid_zone) {
                    mod = fb_avoid_zone * (float)std::signbit(mod_amount);
                }
            }
            fm_amount += op_signal * mod * props.matrix_octave_range;
        }

        auto pitch_adsr               = pitch_adsrs[op_index];
        auto pitch_adsr_depth_octaves = props.pitch_env_mod_depths[op_index];
        float pitch_mod_octaves       = (pitch_adsr.tick() * pitch_adsr_depth_octaves) + fm_amount;

        float op_base_freq = 0;
        auto op_tuning     = props.tunings[op_index];
        auto tuning_type   = props.tuning_types[op_index];
        op_base_freq       = get_tuning(tuning_type, op_tuning);
        float op_freq      = clover::frequency_by_octave_difference(op_base_freq, pitch_mod_octaves);

        op_freq = std::clamp(op_base_freq, 0.f, fs * 0.499f);
        ops[op_index].freq(op_freq);
    }

    // mix signals
    audio_frame output = 0;
    for (auto [op_signal, op_pan, op_gain] : std::views::zip(op_signals, pans, props.op_output_gains)) {
        audio_frame op_signal_stereo = op_pan.process(op_signal);
        output += (op_signal_stereo * op_gain);
    }

    // filter
    float cut_env   = adsr_cut.tick();
    float cut       = props.cut * std::pow(props.cut_mod_target / props.cut, cut_env);
    float res_env   = adsr_res.tick();
    float res       = props.res * std::pow(props.res_mod_target / props.res, res_env);
    filter.m_coeffs = filter_func[int(props.filter_type)](fs, cut, res, 0);

    output = filter.tick(output.L, output.R);

    return output;
}

float fm_037::get_tuning(fm_tuning_type tuning_type, float tuning) {
    switch (tuning_type) {
        case fm_tuning_type::ratio:
            return freq * tuning;
        case fm_tuning_type::hz:
            return tuning;
        default:
            return 0;
    }
}