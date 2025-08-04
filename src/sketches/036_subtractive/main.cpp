// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "behaviours.hpp"
#include "infra/app.hpp"

int main(int, char*[]) {
    app application = app::create<behaviours>();
    application.start();

    return 0;
}
