#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <algorithm>
#include <cmath>
#include <format>
#include <string>
#include <utility>

/*
x => gain => tanh => gain => clip => trim => y
*/
struct driver {
    struct properties {
        float drive_input = 1;
        float drive_clip  = 1;
        float clip_thresh = 1;
        float trim        = 1;

        std::string to_str() {
            return std::format(
                    R"({{
            .drive_input = {}, 
            .drive_clip  = {}, 
            .clip_thresh = {}, 
            .trim        = {}, 
            }};
            )",
                    drive_input,
                    drive_clip,
                    clip_thresh,
                    trim);
        }
    } props;

    std::pair<float, float> tick(const std::pair<float, float>& x) {
        float L = x.first;
        float R = x.second;

        L = std::tanh(L * props.drive_input);
        R = std::tanh(R * props.drive_input);

        L = std::clamp(L * props.drive_clip, -props.clip_thresh, props.clip_thresh) * props.trim;
        R = std::clamp(R * props.drive_clip, -props.clip_thresh, props.clip_thresh) * props.trim;

        return {L, R};
    }
};
