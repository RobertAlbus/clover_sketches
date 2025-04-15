// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <filesystem>
#include <thread>

#include "audio.hpp"
#include "gui.hpp"
#include "shared_props.hpp"

using namespace clover;
using namespace io;

int main(int, char *[]) {
    auto path = std::format("/home/ra/clover/sketches/src/sketches/019_composition/sequence/logs");
    std::filesystem::remove_all(std::filesystem::path(path));
    std::filesystem::create_directories(std::filesystem::path(path));

    shared_props sharedProps;

    std::thread Audio(AUDIO, std::ref(sharedProps));
    std::thread Gui(GUI, std::ref(sharedProps));

    Audio.join();
    Gui.join();

    return 0;
}
