// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "patterns.hpp"

namespace pattern {

std::vector<pattern_t<event>> kick{
        {.duration_bar = 1, .duration_rel = 4., .pattern = {}},
        {.duration_bar = 1,
         .duration_rel = 4.,
         .pattern =
                 {{.start_time = 0, .duration = 0.5},
                  {.start_time = 1, .duration = 0.5},
                  {.start_time = 2, .duration = 0.5},
                  {.start_time = 3, .duration = 0.5}}},
        {.duration_bar = 0.5,
         .duration_rel = 4.,
         .pattern =
                 {{.start_time = 0.0, .duration = 0.5},
                  {.start_time = 1.5, .duration = 0.5},
                  {.start_time = 3.0, .duration = 0.5}}},
};
std::vector<pattern_t<event>> ride{
        {},
        {.duration_bar = 0.25,
         .duration_rel = 1.,
         .pattern =
                 {
                         {.start_time = 0.0, .duration = 0.01},
                         {.start_time = 0.5, .duration = 0.01},
                 }},
};
}  // namespace pattern
