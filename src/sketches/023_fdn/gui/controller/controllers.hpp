#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <vector>

#include "context.hpp"

// TODO: could set up `map<const char*, view_fn> tab_views;`
// then auto populate the tabs into the GUI

struct tabbed_controller {
    const char* name;
    void (*controller)(const char*, context&);
};

extern std::vector<tabbed_controller> tabbed_controllers;
