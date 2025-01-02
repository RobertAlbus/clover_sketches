// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <gtest/gtest.h>

#include "clover_sketches/example.hpp"

TEST(Example, ExampleTest) {
    EXPECT_EQ(clover_sketches::example(), 0);
}