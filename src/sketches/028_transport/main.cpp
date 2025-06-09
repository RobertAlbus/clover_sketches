// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <thread>

#include "audio.hpp"
#include "context.hpp"
#include "gui.hpp"

using namespace clover;
using namespace io;

int main(int, char*[]) {
    context ctx;

    std::thread Audio(AUDIO, std::ref(ctx));
    std::thread Gui(GUI, std::ref(ctx));

    Audio.join();
    Gui.join();

    return 0;
}
