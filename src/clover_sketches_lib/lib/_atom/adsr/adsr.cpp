// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <format>

#include "adsr.hpp"

adsr_values::operator std::string() const {
    return std::format("{{ {}, {}, {}, {} }}", a, d, s, r);
}
