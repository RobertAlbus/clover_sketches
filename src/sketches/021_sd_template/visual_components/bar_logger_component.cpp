// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "imgui.h"

#include "composition/composition.hpp"

#include "visual_components/bar_logger_component.hpp"
#include "visual_components/imgui_app_log.hpp"

void bar_logger_component(composition& comp, bool* p_open) {
    static ExampleAppLog log;
    static int current_bar = -1;

    auto updated_bar = int(comp.counter.progress_bars);

    if (current_bar != updated_bar) {
        current_bar = updated_bar;
        log.AddLog("--------------------------------\nbar: %3d \n", updated_bar);
        log.AddLog("\n");
    }

    ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_FirstUseEver);

    log.Draw("Example: Log", p_open);
}
