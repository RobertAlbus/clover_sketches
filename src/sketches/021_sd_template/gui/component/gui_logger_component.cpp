// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "context.hpp"

#include "gui_logger_component.hpp"
#include "imgui_app_log.hpp"

void gui_logger_component(context& ctx, bool* p_open) {
    static ExampleAppLog log;

    bool has_message = false;
    do {
        gui_log_message msg;
        has_message = ctx.gui_log_queue->try_dequeue(msg);
        if (has_message)
            log.AddLog("%s\n", msg.text);
    } while (has_message);

    ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_FirstUseEver);

    log.Draw("Example: Log", p_open);
}
