#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "context.hpp"

// TODO: could set up `map<const char*, view_fn> tab_views;`
// then auto populate the tabs into the GUI

void controller_mixer(const char* id, context& ctx);
void controller_kick(const char* id, context& ctx);
void controller_chord(const char* id, context& ctx);
