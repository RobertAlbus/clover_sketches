#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "lib/cymbal/cymbal_000.hpp"

struct cymbal_024 : public cymbal_000 {
    using cymbal_000::cymbal_000;

    // tick() is redefined better output gain scaling.
    // avoid modifying existing implementation behaviour
    // in order to preserve the mix of 019_composition
    float tick();
};
