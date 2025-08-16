#include "draw_adsr_knobs_037.hpp"
#include "imgui-knobs.h"
#include "imgui.h"
#include "lib/_atom/resettable.hpp"

bool draw_adsr_knobs_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float& a,
    float& d,
    float& s,
    float& r,
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
    ImGuiKnobs::Knob("", &a, 1.0f, ranges.a_max, 0, format(a), ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(a, 1000.0f);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID("D");
    ImGuiKnobs::Knob("", &d, 1.0f, ranges.d_max, 0, format(d), ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(d, 1000.0f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("S");
    ImGuiKnobs::Knob("", &s, 0.0f, ranges.s_max, 0, "%.2f", ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(s, 0.7f);
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("R");
    ImGuiKnobs::Knob("", &r, 1.0f, ranges.r_max, 0, format(r), ImGuiKnobVariant_Tick, knob_size, knob_flags);
    was_changed |= resettable(r, 1000.0f);
    ImGui::PopID();
    ImGui::EndGroup();

    ImGui::PopID();
    return was_changed;
}
