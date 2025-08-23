#include "draw_adsr_037.hpp"
#include "lib/_molecule/adsr/draw_adsr_038.hpp"

bool draw_adsr_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float& a,
    float& d,
    float& s,
    float& r,
    float graph_width,
    float graph_height,
    float knob_size,
    ImGuiKnobFlags knob_flags) {
    adsr_values adsr_temp = {a, d, s, r};
    bool was_changed = draw_adsr_038(id, ranges, adsr_temp, graph_width, graph_height, knob_size, knob_flags);
    if (was_changed) {
        a = adsr_temp.a;
        d = adsr_temp.d;
        s = adsr_temp.s;
        r = adsr_temp.r;
    }
    return was_changed;
}