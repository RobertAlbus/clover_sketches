#include "draw_adsr_graph_037.hpp"
#include "lib/_atom/adsr/draw_adsr_graph_038.hpp"

void draw_adsr_graph_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float a,
    float d,
    float s,
    float r,
    float width,
    float height) {
    adsr_values adsr_temp = {a, d, s, r};
    draw_adsr_graph_038(id, ranges, adsr_temp, width, height);
}
