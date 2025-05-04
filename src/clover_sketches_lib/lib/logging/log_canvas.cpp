// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

//-----------------------------------------------------------------------------

// NOTICE:
// The following code has been adapted from ImGui's
// demo and I do not assert any claim to the copyright
// of the original material nor the adapated material.

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

#include "imgui.h"

#include "log_canvas.hpp"

log_canvas_000::log_canvas_000() {
    auto_scroll = true;
    clear();
}

void log_canvas_000::clear() {
    buffer.clear();
    line_offsets.clear();
    line_offsets.push_back(0);
}

void log_canvas_000::add_log(const char* fmt, ...) {
    int old_size = buffer.size();
    va_list args;
    va_start(args, fmt);
    buffer.appendfv(fmt, args);
    va_end(args);
    for (int new_size = buffer.size(); old_size < new_size; old_size++)
        if (buffer[old_size] == '\n')
            line_offsets.push_back(old_size + 1);
}

void log_canvas_000::draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &auto_scroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool should_clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    filter.Draw("Filter", -100.0f);

    ImGui::Separator();

    if (ImGui::BeginChild(
                "scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
        if (should_clear)
            clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf     = buffer.begin();
        const char* buf_end = buffer.end();
        if (filter.IsActive()) {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have random access to the result of our filter.
            // A real application processing logs with ten of thousands of entries may want to store the
            // result of search/filter.. especially if the filtering function is not trivial (e.g.
            // reg-exp).
            for (int line_no = 0; line_no < line_offsets.Size; line_no++) {
                const char* line_start = buf + line_offsets[line_no];
                const char* line_end =
                        (line_no + 1 < line_offsets.Size) ? (buf + line_offsets[line_no + 1] - 1) : buf_end;
                if (filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        } else {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will
            // fast-forward to skip non-visible lines. Here we instead demonstrate using the clipper to
            // only process lines that are within the visible area. If you have tens of thousands of items
            // and their processing cost is non-negligible, coarse clipping them on your side is
            // recommended. Using ImGuiListClipper requires
            // - A) random access into your data
            // - B) items all being the  same height,
            // both of which we can handle since we have an array pointing to the beginning of each line
            // of text. When using the filter (in the block of code above) we don't have random access
            // into the data to display anymore, which is why we don't use the clipper. Storing or
            // skimming through the search result would make it possible (and would be recommended if you
            // want to search through tens of thousands of entries).
            ImGuiListClipper clipper;
            clipper.Begin(line_offsets.Size);
            while (clipper.Step()) {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                    const char* line_start = buf + line_offsets[line_no];
                    const char* line_end   = (line_no + 1 < line_offsets.Size)
                                                     ? (buf + line_offsets[line_no + 1] - 1)
                                                     : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of
        // the frame. Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}