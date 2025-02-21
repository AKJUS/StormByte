#pragma once

// This file is used to make the code compatible with different compilers
// while they might lack support for some features

#include <expected>

// Most compilers does not yet support the C++23 throws keyword
#ifndef throws(x)
#define throws(x)
#endif

#ifdef __clang__
    #define CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)

    #if CLANG_VERSION >= 190000 // Check if Clang version is 19.0.0 or higher
        #define CLANG_19_OR_LATER
    #endif
#endif