#pragma once

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

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");

struct log_canvas_000 {
    ImGuiTextBuffer buffer;
    ImGuiTextFilter filter;
    ImVector<int> line_offsets;  // Index to lines offset. We maintain this with AddLog() calls.
    bool auto_scroll;            // Keep scrolling if already at the bottom.

    log_canvas_000();
    void clear();
    void add_log(const char* fmt, ...) IM_FMTARGS(2);
    void draw(const char* title, bool* p_open = nullptr);
};