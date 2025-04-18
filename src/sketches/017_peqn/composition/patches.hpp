#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/instruments/fdn.hpp"
#include "composition/instruments/filter_block.hpp"
#include "composition/instruments/kick.hpp"
#include "composition/instruments/nx_osc.hpp"
#include "composition/instruments/peq.hpp"

extern fdn_8_props_012 patch_deep_fdn;
extern kick_props patch_deep_kick;
extern nx_osc_props patch_deep_chord;
extern filter_block_props patch_deep_chord_filter;

extern fdn_8_props_012 patch_real_fdn;
extern kick_props patch_real_kick;

extern fdn_8_props_012 hand_drum_fdn_patch;
extern kick_props hand_drum_kick_patch;

extern std::array<peq_props, peq::SIZE> kick_peq_props;
