#ifndef GOF2_GAMEDATA_H
#define GOF2_GAMEDATA_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- GameData (top-level class, Android libgof2hdaa.so, armv7 Thumb).
// Constructor and destructor manage 8 embedded AbyssEngine::String members.
//
// Field offsets recovered from the ctor/dtor (Ghidra @0xe0c6c / @0xe06f4):
// the class embeds 8 AbyssEngine::String members at offsets
// 0x18, 0x24, 0x30, 0x7c, 0x88, 0x94, 0xa8, 0xb8.

class GameData {
public:
    AbyssEngine::String field_0x18;     // +0x18
    AbyssEngine::String field_0x24;     // +0x24
    AbyssEngine::String field_0x30;     // +0x30
    AbyssEngine::String field_0x7c;     // +0x7c
    AbyssEngine::String field_0x88;     // +0x88
    AbyssEngine::String field_0x94;     // +0x94
    AbyssEngine::String field_0xa8;     // +0xa8
    AbyssEngine::String field_0xb8;     // +0xb8

    GameData();
    ~GameData();
};
#endif
