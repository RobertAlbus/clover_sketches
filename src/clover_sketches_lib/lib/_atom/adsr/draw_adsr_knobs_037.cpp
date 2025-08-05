#include "draw_adsr_knobs_037.hpp"
#include "imgui-knobs.h"
#include "imgui.h"
#include "lib/_atom/resettable.hpp"

bool draw_adsr_knobs_037(
    const char* id, const adsr_ranges_000& ranges, float& a, float& d, float& s, float& r) {
    ImGui::PushID(id);
    bool was_changed = false;

    ImGui::BeginGroup();

    ImGui::PushID("A");
    ImGuiKnobs::Knob("", &a, 1.0f, ranges.a_max, 0, "", ImGuiKnobVariant_Tick, 30, ImGuiKnobFlags_NoInput);
    was_changed |= resettable(a, 1000.0f);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID("D");
    ImGuiKnobs::Knob("", &d, 1.0f, ranges.d_max, 0, "", ImGuiKnobVariant_Tick, 30, ImGuiKnobFlags_NoInput);
    was_changed |= resettable(d, 1000.0f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("S");
    ImGuiKnobs::Knob("", &s, 0.0f, ranges.s_max, 0, "", ImGuiKnobVariant_Tick, 30, ImGuiKnobFlags_NoInput);
    was_changed |= resettable(s, 0.7f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("R");
    ImGuiKnobs::Knob("", &r, 1.0f, ranges.r_max, 0, "", ImGuiKnobVariant_Tick, 30, ImGuiKnobFlags_NoInput);
    was_changed |= resettable(r, 1000.0f);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::PopID();
    return was_changed;
}
