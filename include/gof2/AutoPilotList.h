#ifndef GOF2_AUTOPILOTLIST_H
#define GOF2_AUTOPILOTLIST_H
#include "gof2/common.h"
// RetStr (12-byte by-value String aggregate) and the SolarSystem class are sourced
// from a single header to avoid the duplicate-RetStr ODR clash that arises when
// SolarSystem.h and Station.h (both define RetStr unconditionally) are pulled into
// one TU. getTargetString() below returns RetStr by value, so its definition must be
// in scope here.
#include "gof2/SolarSystem.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - AutoPilotList class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace; only some arg/field types live in
// AbyssEngine). Field offsets recovered per-method from the target disassembly; we do
// NOT model a full layout - access fields via byte-offset casts from `this`.
//
// Known field offsets:
//   0x00 int    selected index
//   0x04 int    x
//   0x08 int    y
//   0x0c int    width
//   0x10 Array<String*>*  entries (owned; released in dtor)
//   0x14 int    count


struct Level;

// Methods authored as real C++ members demangle to AutoPilotList::name(); the rest are
// extern "C" wrappers with hand-mangled names.




// AbyssEngine::String - 12-byte object. Heap-allocated, constructed via engine helpers.
struct EngString {
    uint32_t a, b, c;
    EngString(const void *src, bool copy);   // String(String*, bool) / String(char*, bool)
};
void *operator new(__SIZE_TYPE__);

// RetStr is provided by gof2/SolarSystem.h (included above).

// Field accessor via byte offset.

struct AutoPilotList {
    int field_0x0;                      // +0x0  selected index
    int field_0x4;                      // +0x4  x
    int field_0x8;                      // +0x8  y
    int field_0xc;                      // +0xc  width
    void* field_0x10;                   // +0x10 Array<String*>* entries
    int field_0x14;                     // +0x14 count

    void draw();

    // ---- methods (converted from free functions) ----
    RetStr getTargetString();
    int touch(int p1, int p2);
};
#endif
