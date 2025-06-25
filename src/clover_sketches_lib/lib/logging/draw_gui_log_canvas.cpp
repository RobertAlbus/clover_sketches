// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "log_canvas.hpp"
#include "logger.hpp"

void draw_gui_log_canvas_000(const char* id, log_canvas_000& canvas, log_bus_000& logger, bool* p_open) {
    ImGui::PushID(id);

    bool has_message = false;
    do {
        log_message_000 msg;
        has_message = logger.gui.try_dequeue(msg);
        if (has_message)
            canvas.add_log("%s\n", msg.text);
    } while (has_message);

    ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_FirstUseEver);

    canvas.draw("Logs", p_open);

    ImGui::PopID();
}
