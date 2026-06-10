#ifndef GOF2_LISTITEM_H
#define GOF2_LISTITEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct ListItem {
    Agent* field_0x4;                   // +0x4
    BluePrint* field_0x8;               // +0x8
    Ship* field_0xc;                    // +0xc
    Item* field_0x10;                   // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    void* field_0x1c;                   // +0x1c
    void* field_0x20;                   // +0x20
    uint8_t field_0x24;                 // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    uint8_t field_0x38;                 // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    uint8_t field_0x44;                 // +0x44
};
#endif
