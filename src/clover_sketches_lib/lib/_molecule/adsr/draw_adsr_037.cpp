#include "imgui.h"

#include "lib/_atom/adsr/draw_adsr_graph_037.hpp"
#include "lib/_atom/adsr/draw_adsr_knobs_037.hpp"

#include "draw_adsr_037.hpp"

bool draw_adsr_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float& a,
    float& d,
    float& s,
    float& r,
    float width,
    float height) {
    ImGui::PushID(id);
    bool was_changed = false;

    draw_adsr_graph_037("##graph", ranges, a, d, s, r, width, height);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 15);
    was_changed |= draw_adsr_knobs_037("##knobs", ranges, a, d, s, r);

    ImGui::PopID();
    return was_changed;
}