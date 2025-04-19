#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "composition/composition.hpp"
#include "instruments/frsq.hpp"
#include <memory>

struct sequencer_entry {
    std::string name;
    std::shared_ptr<frsq_base> frsq;
    std::function<void(sequencer_entry&, composition&)> configure;
};

struct sequencers {
    std::vector<sequencer_entry> sqs;

    sequencers(composition& comp);
    void tick();
};