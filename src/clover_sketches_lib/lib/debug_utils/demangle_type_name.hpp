#pragma once

// Sketches with Clover Audio Framework
// Copyright (C) 2025  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.
#include <cstdlib>
#include <cxxabi.h>
#include <string>
#include <typeinfo>

// WARNING: malloc from abi::__cxa_demangle
template <typename T>
std::string demangle_type_name() {
    int status         = 0;
    char* demangled    = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    std::string result = (status == 0) ? demangled : typeid(T).name();
    free(demangled);
    return result;
}

// WARNING: malloc from abi::__cxa_demangle
template <typename T>
std::string demangle_type_name(T& thing) {
    return demangle_type_name<decltype(thing)>();
}

// WARNING: malloc from abi::__cxa_demangle
template <typename T>
std::string demangle_type_name(T* thing) {
    return demangle_type_name<decltype(*thing)>();
}
