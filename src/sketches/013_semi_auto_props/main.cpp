// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <cmath>
#include <print>

struct Settable {
    float value = 0;
    void set(float val) {
        value = val;
    }
    void tick() const {
        std::println("Settable::tick() called. value: {}.", value);
    }
};

struct PropsBase {
    void tick() {
        if (settables == nullptr)
            register_settables();
        for (int i = 0; i < settable_count(); ++i)
            settables[i].tick();
    }
    virtual void register_settables()  = 0;
    Settable* settables                = nullptr;
    virtual const int settable_count() = 0;
};
struct ExampleProps1 : public PropsBase {
    Settable a;
    Settable b;
    Settable c;

    static constexpr int count = 3;
    std::array<Settable*, count> settable_array{&a, &b, &c};
    const int settable_count() override {
        return count;
    }
    void register_settables() override {
        settables = (settable_array[0]);
    }
};

struct ExampleProps2 : public PropsBase {
    Settable d{4};
    Settable e{5};
    Settable f{6};
    Settable g{7};

    static constexpr int count = 4;
    std::array<Settable*, count> settable_array{&d, &e, &f, &g};
    const int settable_count() override {
        return count;
    }
    void register_settables() override {
        settables = (settable_array[0]);
    }
};

// Initialization of non-aggregate type 'ExampleProps2' with a designated initializer list
// clang(designated_init_for_non_aggregate)
// ExampleProps2 preset_example_props2{
//         .d = 10,
//         .e = 11,
//         .f = 12,
//         .g = 13,
// };
//
// Would need to encode and decode the presets from strings to get around this.
// Consider Boost.PFR

int main(int argc, char* argv[]) {
    ExampleProps1 example_props1;
    ExampleProps2 example_props2;
    example_props1.tick();
    example_props2.tick();

    exit(0);
}
