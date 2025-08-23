// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <ranges>

#include "ducker.hpp"

ducker::ducker(ducker_props& props) {
    patch(props);
}

void ducker::patch(ducker_props new_props) {
    props = new_props;
    envs.clear();
    envs.reserve(new_props.patterns.size());
    xs.clear();
    xs.reserve(new_props.patterns.size());

    for (auto& pattern : props.patterns) {
        envs.emplace_back(std::span(pattern));
        xs.emplace_back(0);
    }
    for (auto& env : envs) {
        env.duration_abs = props.duration_abs;
        env.duration_rel = props.duration_rel;
    }
}

void ducker::key_on() {
    for (auto& env : envs) {
        env.key_on();
    }
}

void ducker::tick() {
    for (auto i : std::views::iota(0u, envs.size())) {
        xs[i] = envs[i].tick();
    }
}
