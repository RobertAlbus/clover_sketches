// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <functional>
#include <utility>

#include "imgui.h"

#include "transport_ui_028.hpp"

transport_ui_028::transport_ui_028(
    std::function<void(float)> start_action,
    std::function<void()> play_action,
    std::function<void()> stop_action)
    : start_action{std::move(start_action)},
      play_action{std::move(play_action)},
      stop_action{std::move(stop_action)} {
}
void transport_ui_028::draw() {
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("##bar", &bar, 0.1f, 0, 31, "%.1f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button(is_playing ? "stop" : "play")) {
        toggle_state_start();
    }
}

void transport_ui_028::toggle_state_start() {
    is_playing ? stop_action() : start_action(bar);
    is_playing = !is_playing;
}

void transport_ui_028::toggle_state_play() {
    is_playing ? stop_action() : play_action();
    is_playing = !is_playing;
}