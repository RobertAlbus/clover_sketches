// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <ranges>

#include "ducker.hpp"

ducker::ducker(ducker_props& props) {
    patch(props);
}

void ducker::patch(ducker_props new_props) {
    props = new_props;
    envs.clear();
    envs.reserve(new_props.envelopes.size());
    envs_kick.clear();
    envs_kick.reserve(new_props.envelopes.size());

    xs.clear();
    xs.resize(new_props.envelopes.size());
    std::fill(xs.begin(), xs.end(), 0);
    xs_kick.clear();
    xs_kick.resize(new_props.envelopes.size());
    std::fill(xs_kick.begin(), xs_kick.end(), 0);

    for (auto& envelope : props.envelopes) {
        envs.emplace_back(std::span(envelope));
        envs_kick.emplace_back(std::span(envelope));
    }
    for (auto& env : std::views::concat(envs, envs_kick)) {
        env.duration_abs = props.duration_abs;
        env.duration_rel = props.duration_rel;
    }
}

void ducker::key_on() {
    for (auto& env : envs) {
        env.key_on();
    }
}

void ducker::key_on_kick() {
    for (auto& env : envs_kick) {
        env.key_on();
    }
}

void ducker::tick() {
    for (auto [x, x_kick, env, env_kick] : std::views::zip(xs, xs_kick, envs, envs_kick)) {
        x      = env.tick();
        x_kick = env_kick.tick();
    }
}
