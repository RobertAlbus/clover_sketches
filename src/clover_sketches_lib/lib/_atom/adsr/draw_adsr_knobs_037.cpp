#include "draw_adsr_knobs_037.hpp"
#include "lib/_atom/adsr/draw_adsr_knobs_038.hpp"

bool draw_adsr_knobs_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float& a,
    float& d,
    float& s,
    float& r,
    float knob_size,
    ImGuiKnobFlags knob_flags) {
    adsr_values adsr_temp = {a, d, s, r};
    bool was_changed      = draw_adsr_knobs_038(id, ranges, adsr_temp, knob_size, knob_flags);
    if (was_changed) {
        a = adsr_temp.a;
        d = adsr_temp.d;
        s = adsr_temp.s;
        r = adsr_temp.r;
    }
    return was_changed;
}
