#ifndef GOF2_GAMERECORD_H
#define GOF2_GAMERECORD_H
#include <cstddef>
#include <cstdint>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"

#pragma pack(push, 1)
class GameRecord {
public:
    uint8_t *data;
    uint32_t field_0x04;
    uint32_t field_0x08;
    uint32_t field_0x0c;

    // 0x10..0x17 is read both as a {pointer, uint32} pair and as a single int64 playTime
    // (RecordHandler.cpp). Kept as the two real members; the int64 view is
    // reinterpret_cast<int64_t &>(playTimeObj) at the few sites that need it.
    uint8_t *playTimeObj;
    uint32_t field_0x14;

    uint32_t field_0x18;
    uint32_t field_0x1c;
    uint32_t killsText;
    uint32_t field_0x24;
    uint32_t field_0x28;
    uint32_t field_0x2c;
    uint32_t field_0x30;
    uint32_t field_0x34;
    uint32_t field_0x38;
    uint32_t field_0x3c;
    uint32_t field_0x40;
    uint32_t field_0x44;
    uint32_t field_0x48;
    uint32_t field_0x4c;
    uint32_t field_0x50;
    uint32_t field_0x54;
    uint32_t field_0x58;
    uint32_t field_0x5c;
    uint32_t field_0x60;
    uint32_t field_0x64;
    uint32_t field_0x68;
    uint32_t field_0x6c;
    uint32_t field_0x70;
    uint32_t field_0x74;
    uint32_t field_0x78;
    uint32_t field_0x7c;
    uint32_t field_0x80;
    uint32_t field_0x84;
    uint32_t field_0x88;
    uint32_t field_0x8c;
    uint32_t field_0x90;
    uint32_t field_0x94;
    uint32_t field_0x98;
    uint32_t field_0x9c;
    uint32_t field_0xa0;
    uint32_t field_0xa4;
    uint32_t field_0xa8;
    uint32_t field_0xac;
    uint32_t field_0xb0;
    uint32_t field_0xb4;
    uint32_t field_0xb8;
    uint32_t field_0xbc;
    uint32_t field_0xc0;
    uint32_t field_0xc4;
    uint32_t field_0xc8;
    uint32_t field_0xcc;
    uint32_t field_0xd0;
    uint8_t field_0xd4;
    uint8_t field_0xd5;
    uint8_t field_0xd6;
    uint8_t field_0xd7;
    uint32_t field_0xd8;
    uint8_t field_0xdc;
    uint8_t field_0xdd;
    uint8_t field_0xde;
    uint8_t field_0xdf;
    uint32_t field_0xe0;
    uint32_t field_0xe4;
    uint32_t field_0xe8;
    uint32_t field_0xec;
    uint32_t field_0xf0;
    uint32_t field_0xf4;
    uint32_t field_0xf8;
    uint32_t field_0xfc;

    uint8_t field_0x100;
    uint8_t field_0x101;
    uint8_t field_0x102;
    uint8_t field_0x103;
    uint8_t field_0x104;
    uint8_t field_0x105;
    uint8_t field_0x106;
    uint8_t field_0x107;
    uint8_t field_0x108;
    uint8_t field_0x109;
    uint8_t field_0x10a;
    uint8_t field_0x10b;
    uint8_t field_0x10c;
    uint8_t field_0x10d;
    uint8_t field_0x10e;
    uint8_t field_0x10f;
    uint8_t field_0x110;
    uint8_t field_0x111;
    uint8_t field_0x112;
    uint8_t field_0x113;
    uint8_t field_0x114;
    uint8_t dlcRequiredFlag;
    uint8_t field_0x116;
    uint8_t versionMismatchFlag;
    uint8_t field_0x118;
    uint8_t field_0x119;
    uint8_t field_0x11a;
    uint8_t field_0x11b;

    // 0x11c is the float rank; RecordHandler.cpp also writes its raw bits via
    // reinterpret_cast<int32_t &>(rank).
    float rank;

    uint32_t field_0x120;
    uint32_t field_0x124;
    uint32_t field_0x128;
    uint32_t field_0x12c;
    uint32_t field_0x130;
    uint32_t field_0x134;
    uint32_t field_0x138;
    uint32_t field_0x13c;
    uint32_t field_0x140;
    uint32_t field_0x144;
    uint32_t field_0x148;
    uint32_t field_0x14c;
    uint32_t field_0x150;
    uint32_t field_0x154;
    uint32_t field_0x158;
    uint32_t field_0x15c;
    uint32_t field_0x160;
    uint32_t field_0x164;
    uint32_t field_0x168;
    uint32_t field_0x16c;
    uint32_t field_0x170;
    uint32_t field_0x174;
    uint32_t field_0x178;
    uint32_t field_0x17c;
    uint32_t field_0x180;
    uint32_t field_0x184;
    uint32_t field_0x188;
    uint32_t field_0x18c;
    uint32_t field_0x190;
    uint32_t pilotName;
    uint32_t field_0x198;
    uint32_t field_0x19c;
    uint32_t shipId;
    uint32_t field_0x1a4;
    uint32_t field_0x1a8;
    uint32_t field_0x1ac;
    uint32_t field_0x1b0;
    uint32_t field_0x1b4;
    uint32_t field_0x1b8;
    uint32_t field_0x1bc;
    uint8_t field_0x1c0;

    GameRecord();

    ~GameRecord();

    void load();
};
#pragma pack(pop)

#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(GameRecord) == 449, "GameRecord must be 449 bytes (0x1c1)");
static_assert(offsetof(GameRecord, data) == 0x0, "");
static_assert(offsetof(GameRecord, field_0x98) == 0x98, "");
static_assert(offsetof(GameRecord, field_0xa8) == 0xa8, "");
static_assert(offsetof(GameRecord, field_0xc8) == 0xc8, "");
static_assert(offsetof(GameRecord, field_0xd4) == 0xd4, "");
static_assert(offsetof(GameRecord, field_0xd8) == 0xd8, "");
static_assert(offsetof(GameRecord, field_0xdc) == 0xdc, "");
static_assert(offsetof(GameRecord, field_0xe0) == 0xe0, "");
static_assert(offsetof(GameRecord, field_0x100) == 0x100, "");
static_assert(offsetof(GameRecord, field_0x102) == 0x102, "");
static_assert(offsetof(GameRecord, dlcRequiredFlag) == 0x115, "dlcRequiredFlag");
static_assert(offsetof(GameRecord, versionMismatchFlag) == 0x117, "versionMismatchFlag");
static_assert(offsetof(GameRecord, playTimeObj) == 0x10, "playTimeObj");
static_assert(offsetof(GameRecord, field_0x14) == 0x14, "field_0x14");
static_assert(offsetof(GameRecord, field_0x18) == 0x18, "field_0x18");
static_assert(offsetof(GameRecord, killsText) == 0x20, "killsText");
static_assert(offsetof(GameRecord, pilotName) == 0x194, "pilotName");
static_assert(offsetof(GameRecord, field_0x119) == 0x119, "");
static_assert(offsetof(GameRecord, rank) == 0x11c, "");
static_assert(offsetof(GameRecord, field_0x120) == 0x120, "");
static_assert(offsetof(GameRecord, field_0x130) == 0x130, "");
static_assert(offsetof(GameRecord, shipId) == 0x1a0, "");
static_assert(offsetof(GameRecord, field_0x1b4) == 0x1b4, "");
static_assert(offsetof(GameRecord, field_0x1b8) == 0x1b8, "");
static_assert(offsetof(GameRecord, field_0x1c0) == 0x1c0, "");
#endif

#endif
