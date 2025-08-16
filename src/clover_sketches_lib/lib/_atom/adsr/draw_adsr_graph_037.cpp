#include "draw_adsr_graph_037.hpp"
#include "imgui.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <ranges>

void draw_adsr_graph_037(
    const char* id,
    const adsr_ranges_000& ranges,
    float a,
    float d,
    float s,
    float r,
    float width,
    float height) {
    ImGui::PushID(id);

    auto time_to_display = [](float time_ms, float max_ms) -> float {
        if (time_ms <= 1.0f)
            return 0.0f;
        float normalized = time_ms / max_ms;
        return std::log(1.0f + normalized * 9.0f) / std::log(10.0f);
    };

    ImVec2 canvas_pos     = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size    = ImVec2(width, height);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // apply log scaling for time values
    float a_display = time_to_display(a, ranges.a_max);
    float d_display = time_to_display(d, ranges.d_max);
    float r_display = time_to_display(r, ranges.r_max);

    // normalize for total display width
    float total_display = a_display + d_display + 0.3f + r_display;  // 0.3f for sustain visual
    float scale         = std::max(total_display, 0.8f);

    float sus_length = 0.1f;
    std::array<float, 5> x_points{
        0.0f,
        a_display / scale,
        (a_display + d_display) / scale,
        (a_display + d_display + sus_length) / scale,
        (a_display + d_display + sus_length + r_display) / scale};
    std::array<float, 5> y_points{0.0f, 1.0f, s / ranges.s_max, s / ranges.s_max, 0.0f};
    for (auto& x : y_points) {
        x *= 0.9f;
    }

    // convert to screen coordinates
    std::array<ImVec2, 5> screen_points;
    for (auto [p, x, y] : std::views::zip(screen_points, x_points, y_points)) {
        p.x = canvas_pos.x + x * canvas_size.x;
        p.y = canvas_pos.y + (1.0f - y) * canvas_size.y;
    }

    ImU32 line_color = ImGui::GetColorU32(ImGuiCol_PlotLines);
    for (auto [a, b] : screen_points | std::views::adjacent<2>) {
        draw_list->AddLine(a, b, line_color, 1.0f);
    }

    // Draw points
    ImU32 point_color = ImGui::GetColorU32(ImGuiCol_ButtonActive);
    for (auto p : screen_points | std::views::drop(1) | std::views::take(3)) {
        draw_list->AddCircleFilled(p, 3.0f, point_color);
    }

    // ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border, 2);
    // draw_list->AddRect(
    //     canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), border_color);

    // Reserve space for the canvas
    ImGui::Dummy(canvas_size);

    ImGui::PopID();
}