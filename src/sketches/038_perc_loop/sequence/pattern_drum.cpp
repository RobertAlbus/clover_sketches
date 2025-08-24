// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"
#include <vector>
// #include "sequence/event.hpp"

std::vector<pattern_t<event>> patterns::create_pattern_kick() {
    return {

        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //
                 {.start_time = 0, .duration = 0.5},
                 {.start_time = 1, .duration = 0.5},
                 {.start_time = 2, .duration = 0.5},
                 {.start_time = 3, .duration = 0.5},
             }},
        {.duration_bar = 1,
         .duration_rel = 16,
         .pattern =
             {
                 //
                 {.start_time = 0, .duration = 0.5},
                 //  {.start_time = 3, .duration = 0.5},
                 {.start_time = 6, .duration = 0.5},
                 //  {.start_time = 10, .duration = 0.5},
                 //  {.start_time = 12, .duration = 0.5},
                 {.start_time = 14, .duration = 0.5},
             }},
    };
}
std::vector<pattern_t<event>> patterns::create_pattern_snare() {
    return {

        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //
                 {.start_time = 1, .duration = 0.5},
                 {.start_time = 3, .duration = 0.5},
             }},
        {.duration_bar = 1,
         .duration_rel = 4,
         .pattern =
             {
                 //
                 {.start_time = 1, .duration = 1},
                 {.start_time = 2.5, .duration = 1},
             }},
    };
}
std::vector<pattern_t<event>> patterns::create_pattern_ride() {
    return {
        {.duration_bar = 1, .duration_rel = 1., .pattern = {}},
        {.duration_bar = 0.25,
         .duration_rel = 1.,
         .pattern =
             {
                 //
                 {.start_time = 0.0, .duration = 0.01},
                 {.start_time = 0.5, .duration = 0.01},
             }},
    };
}

std::vector<pattern_t<event>> patterns::create_pattern_hh1() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //
                 {.start_time = 0.50, .duration = 0.25},
                 {.start_time = 1.50, .duration = 0.20},
                 {.start_time = 1.75, .duration = 0.25},
             }},
    };
}
std::vector<pattern_t<event>> patterns::create_pattern_hh2() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //
                 {.start_time = 0.002, .duration = 0.1},
                 {.start_time = 0.252, .duration = 0.1},
                 {.start_time = 0.502, .duration = 0.1},
                 {.start_time = 1.002, .duration = 0.1},
                 {.start_time = 1.502, .duration = 0.1},
             }},
    };
}
std::vector<pattern_t<event>> patterns::create_pattern_hh3() {
    return {
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
             {
                 //
                 {.start_time = 0.05, .duration = 0.4},
                 {.start_time = 0.55, .duration = 0.4},
                 {.start_time = 1.05, .duration = 0.4},
                 {.start_time = 1.55, .duration = 0.4},
                 {.start_time = 2.05, .duration = 0.4},
                 {.start_time = 2.55, .duration = 0.4},
                 {.start_time = 3.05, .duration = 0.4},
                 {.start_time = 3.55, .duration = 0.4},
             }},
    };
}
