#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/composition.hpp"

// TODO: could set up `map<const char*, view_fn> tab_views;`
// then auto populate the tabs into the GUI

void view_mixer(const char* id, composition* comp);
void view_kick(const char* id, composition* comp);
void view_chord(const char* id, composition* comp);
