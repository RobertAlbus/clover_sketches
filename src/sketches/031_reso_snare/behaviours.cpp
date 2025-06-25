// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "graph/graph.hpp"
#include "lib/cymbal/cymbal_024.hpp"
#include "lib/logging/logger.hpp"
#include "lib/sq/bar_grid_029.hpp"
#include "sequence/sequencers.hpp"
#include "sequence/set_up_sequencing.hpp"

std::vector<frsq_pair> build_frsq_pairs(
        signal_graph& graph,
        bar_grid_029& grid,
        log_bus_000& log,
        patterns& patterns,
        arrangement& arrangement) {
    std::vector<frsq_pair> frsq_pairs;
    frsq_pairs.emplace_back(create_sequencers(
            std::span<kick_drum_000>(&graph.kick, 1),
            [](kick_drum_000& voice, const event& data) { voice.key_on(); },
            [](kick_drum_000& voice) { voice.key_off(); },
            log,
            grid,
            patterns.kick,
            arrangement.kick,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_kick"));
    frsq_pairs.emplace_back(create_sequencers(
            std::span<cymbal_024>(&graph.snare_impulse, 1),
            [](cymbal_024& voice, const event& data) {
                for (auto& osc : voice.oscs)
                    osc.phase(0);
                voice.key_on();
            },
            [](cymbal_024& voice) { voice.key_off(); },
            log,
            grid,
            patterns.snare,
            arrangement.snare,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_snare"));
    frsq_pairs.emplace_back(create_sequencers(
            std::span<cymbal_024>(&graph.ride, 1),
            [](cymbal_024& voice, const event& data) { voice.key_on(); },
            [](cymbal_024& voice) { voice.key_off(); },
            log,
            grid,
            patterns.ride,
            arrangement.ride,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_ride"));
    frsq_pairs.emplace_back(create_sequencers(
            std::span<subtractive_synth_000>(graph.chord),
            [](subtractive_synth_000& voice, const event_midi& data) { voice.key_on(data.note); },
            [](subtractive_synth_000& voice) { voice.key_off(); },
            log,
            grid,
            patterns.chord,
            arrangement.chord,
            grid.duration_bars,
            grid.bars_to_samples(grid.duration_bars),
            "frsq_chord"));

    return frsq_pairs;
}
