// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "frsq.hpp"
#include "composition/instruments/subtractive_synth.hpp"
#include "composition/patterns/patterns.hpp"

template struct frsq<pattern::midi_event, subtractive_synth>;
