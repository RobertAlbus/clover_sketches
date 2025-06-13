// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "app.hpp"
#include "context.hpp"

int main(int, char*[]) {
    app application = app::create<context_029>();
    application.start();

    return 0;
}
