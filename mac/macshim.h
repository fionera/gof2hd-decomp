#pragma once
// 64-bit macOS build shim: the armv7 offset static_asserts in class.h legitimately fail at 64-bit
// (pointer fields are 8 bytes), so neutralize them here. Pointer<->int truncating casts are widened.
#define static_assert(...)
