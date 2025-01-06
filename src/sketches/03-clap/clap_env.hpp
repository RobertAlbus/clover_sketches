#pragma once

#include "clover/dsp/env_linear.hpp"
#include <map>
#include <ranges>

struct trigger_info {
    int start_time;
    float env_duration;
};
struct clap_envelope {
    clover::dsp::env_linear env;
    int duration;
    int count;

    std::map<int, float> triggers;

    clap_envelope(int pulse_count, int spacing, int duration, int duration_final) {
        count = ((pulse_count - 1) * spacing) + duration_final;
        for (auto i : std::views::iota(0, pulse_count))
            triggers.insert({i * spacing, float(i == pulse_count - 1 ? duration_final : duration)});
    }

    clover_float tick() {
        if (triggers.contains(count)) {
            env.set(1, 0, triggers.at(count));
        }
        ++count;

        return env.tick();
    }

    void trigger() {
        count = 0;
    }
};