#ifndef GOF2_NEWSTICKER_H
#define GOF2_NEWSTICKER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
struct String;
}
typedef AbyssEngine::String String;



namespace AbyssEngine {


String operator+(const String &left, const String &right);
}

struct NewsTicker {
    float field_0x0;                    // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    uint8_t field_0x28;                 // +0x28
    int field_0x2c;                     // +0x2c
};
#endif
