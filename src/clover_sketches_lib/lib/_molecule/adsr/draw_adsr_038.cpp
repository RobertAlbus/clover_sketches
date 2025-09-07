#include "imgui-knobs.h"
#include "imgui.h"

#include "lib/_atom/adsr/draw_adsr_graph_038.hpp"
#include "lib/_atom/adsr/draw_adsr_knobs_038.hpp"

#include "draw_adsr_038.hpp"

bool draw_adsr_038(
    const char* id,
    const adsr_ranges_000& ranges,
    adsr_values& adsr,
    float graph_width,
    float graph_height,
    float knob_size,
    ImGuiKnobFlags knob_flags) {
    ImGui::BeginGroup();  // bounding group

    ImGui::PushID(id);

    draw_adsr_graph_038("##graph", ranges, adsr, graph_width, graph_height);
    bool was_changed = draw_adsr_knobs_038("##knobs", ranges, adsr, knob_size, knob_flags);

    ImGui::PopID();

    ImGui::EndGroup();  // bounding group
    return was_changed;
}
