#include "draw_adsr_knobs_038.hpp"
#include "imgui-knobs.h"
#include "imgui.h"
#include "lib/_atom/resettable.hpp"

bool draw_adsr_knobs_038(
    const char* id,
    const adsr_ranges_000& ranges,
    adsr_values& adsr,
    float knob_size,
    ImGuiKnobFlags knob_flags) {
    ImGui::PushID(id);
    bool was_changed = false;

    ImGui::BeginGroup();

    auto format = [](float value) -> const char* {
        if (value < 100) {
            return "%.1f";
        }
        if (value < 10) {
            return "%.2f";
        }
        return "%.0f";
    };

    ImGui::PushID("A");
    ImGuiKnobs::Knob(
        "", &adsr.a, 1.0f, ranges.a_max, 0, format(adsr.a), ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(adsr.a, 1000.0f);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID("D");
    ImGuiKnobs::Knob(
        "", &adsr.d, 1.0f, ranges.d_max, 0, format(adsr.d), ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(adsr.d, 1000.0f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("S");
    ImGuiKnobs::Knob(
        "", &adsr.s, 0.0f, ranges.s_max, 0, "%.2f", ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(adsr.s, 0.7f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("R");
    ImGuiKnobs::Knob(
        "", &adsr.r, 1.0f, ranges.r_max, 0, format(adsr.r), ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(adsr.r, 1000.0f);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::PopID();
    return was_changed;
}
