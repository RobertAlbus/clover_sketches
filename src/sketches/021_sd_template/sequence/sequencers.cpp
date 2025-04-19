// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <memory>
#include <print>

#include "composition/composition.hpp"
#include "instruments/kick.hpp"
#include "instruments/subtractive_synth.hpp"
#include "sequence/event.hpp"
#include "sequence/patterns.hpp"
#include "sequencers.hpp"
std::vector<sequencer_entry> make_sequencers_vector() {
    std::vector<sequencer_entry> sqs;

    auto& kick_entry = sqs.emplace_back(sequencer_entry{
            .name      = "kick",
            .frsq      = std::make_shared<frsq<kick_drum, event>>(),
            .configure = [](sequencer_entry& entry, composition& comp) {
                frsq<kick_drum, event>& frsq_kick =
                        *std::dynamic_pointer_cast<frsq<kick_drum, event>>(entry.frsq);

                frsq_kick.voices            = std::span<kick_drum>(&comp.kick.drum, 1);
                frsq_kick.duration_absolute = comp.sp_bar;
                frsq_kick.duration_relative = 4.0;
                frsq_kick.callback_start    = [](kick_drum& v, const event&) { v.key_on(); };
                frsq_kick.callback_end      = [](kick_drum& v) { v.key_off(); };
            }});

    auto chord_entry = sqs.emplace_back(sequencer_entry{
            .name = "chord",
            .frsq = std::make_shared<frsq<subtractive_synth, event_midi>>(),
            .configure =
                    [](sequencer_entry& entry, composition& comp) {
                        frsq<subtractive_synth, event_midi>& frsq_chord =
                                *std::dynamic_pointer_cast<frsq<subtractive_synth, event_midi>>(entry.frsq);

                        frsq_chord.voices = std::span(comp.synth.chord.begin(), comp.synth.chord.end());
                        frsq_chord.duration_absolute = comp.sp_bar;
                        frsq_chord.duration_relative = 4.0;
                        frsq_chord.callback_start    = [](subtractive_synth& v, const event_midi& e) {
                            v.key_on(e.note);
                        };
                        frsq_chord.callback_end = [](subtractive_synth& v) { v.key_off(); };
                    }

    });

    sqs.emplace_back(sequencer_entry{
            .name      = "kick_metasq",
            .frsq      = std::make_shared<frsq<frsq<kick_drum, event>, event_meta_sq>>(),
            .configure = [&](sequencer_entry& entry, composition& comp) {
                auto& meta_kick =
                        *std::dynamic_pointer_cast<frsq<frsq<kick_drum, event>, event_meta_sq>>(entry.frsq);
                auto& frsq_kick  = *std::dynamic_pointer_cast<frsq<kick_drum, event>>(kick_entry.frsq);
                meta_kick.voices = std::span(&frsq_kick, 1);
                meta_kick.duration_absolute = comp.sp_bar * comp.duration_bars;
                meta_kick.duration_relative = comp.duration_bars;
                meta_kick.callback_start    = [&](frsq<kick_drum, event>& v, const event_meta_sq& e) {
                    if (comp.gui_log_queue) {
                        gui_log_message msg;
                        snprintf(
                                msg.text,
                                sizeof(msg.text),
                                " - %s:    %zu @ %f",
                                entry.name.c_str(),
                                e.pattern_index,
                                e.start_time);
                        if (!comp.gui_log_queue->try_enqueue(msg)) {
                            std::println("{}.callback_start failed to log to gui", entry.name);
                        }
                    }
                    v.set_pattern(pattern::kick[e.pattern_index]);
                };
                meta_kick.set_pattern(arrangement::kick, arrangement::playback_start);
            }});

    sqs.emplace_back(sequencer_entry{
            .name      = "chord_metasq",
            .frsq      = std::make_shared<frsq<frsq<subtractive_synth, event_midi>, event_meta_sq>>(),
            .configure = [&](sequencer_entry& entry, composition& comp) {
                auto& meta =
                        *std::dynamic_pointer_cast<frsq<frsq<subtractive_synth, event_midi>, event_meta_sq>>(
                                entry.frsq);
                auto& target =
                        *std::dynamic_pointer_cast<frsq<subtractive_synth, event_midi>>(chord_entry.frsq);
                meta.voices            = std::span(&target, 1);
                meta.duration_absolute = comp.sp_bar * comp.duration_bars;
                meta.duration_relative = comp.duration_bars;
                meta.callback_start    = [&](frsq<subtractive_synth, event_midi>& v, const event_meta_sq& e) {
                    if (comp.gui_log_queue) {
                        gui_log_message msg;
                        snprintf(
                                msg.text,
                                sizeof(msg.text),
                                " - %s:    %zu @ %f",
                                entry.name.c_str(),
                                e.pattern_index,
                                e.start_time);
                        if (!comp.gui_log_queue->try_enqueue(msg)) {
                            std::println("{}.callback_start failed to log to gui", entry.name);
                        }
                    }
                    v.set_pattern(pattern::chord[e.pattern_index]);
                };
                meta.set_pattern(arrangement::chord, arrangement::playback_start);
            }});

    sqs.emplace_back(sequencer_entry{
            .name      = "bar_print",
            .frsq      = std::make_shared<frsq<event, event>>(),
            .configure = [&](sequencer_entry& entry, composition& comp) {
                auto& target             = *std::dynamic_pointer_cast<frsq<event, event>>(entry.frsq);
                target.voices            = std::span(arrangement::bar.begin(), 1);
                target.duration_absolute = comp.sp_bar * comp.duration_bars;
                target.duration_relative = comp.duration_bars;
                target.set_pattern(arrangement::bar);

                target.callback_start = [&](event&, const event& e) {
                    if (comp.gui_log_queue) {
                        gui_log_message msg;
                        snprintf(msg.text, sizeof(msg.text), "\n--------\n bar: %d", int(e.start_time));
                        if (!comp.gui_log_queue->try_enqueue(msg)) {
                            std::println("arrangement print failed to log to gui");
                        }
                    }
                };
            }});

    return sqs;
}

sequencers::sequencers(composition& comp) {
    for (auto& sq : sqs)
        sq.configure(sq, comp);
}

void sequencers::tick() {
    for (auto& sq : sqs)
        sq.frsq->tick();
}
