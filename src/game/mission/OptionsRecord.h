#ifndef GOF2_OPTIONSRECORD_H
#define GOF2_OPTIONSRECORD_H
#include <cstddef>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "GameRecord.h"
#include "Mission.h"
#include "engine/render/Engine.h"
#include "game/world/Wanted.h"

struct OptionsRecord {
    uint8_t field_0x00[8];
    uint16_t flag_word_0x8;
    uint8_t flag_0xa;
    uint8_t field_0x0b[2];

    uint16_t flag_word_0xd;

    uint8_t flag_0xf;
    uint8_t field_0x10[3];
    uint8_t flag_0x13;
    uint8_t field_0x14;
    uint8_t flag_0x15;
    uint8_t field_0x16;
    uint8_t flag_0x17;
    uint8_t field_0x18[4];
    uint16_t flag_word_0x1c;
    uint8_t flag_0x1e;
    uint8_t field_0x1f;
    int32_t flag_dword_0x20;
    uint8_t flag_0x24;
    uint8_t field_0x25;
    uint8_t flag_0x26;
    uint8_t field_0x27[5];
    uint32_t fadeValue;
    uint8_t field_0x30;
    uint8_t flag_0x31;

    // This 4-byte slot is zeroed whole (formerly flag_dword_0x32) in MenuTouchWindow.cpp and also
    // accessed as the named bytes flag_0x34/flag_0x35. Kept as the bytes; the whole-word write is
    // reinterpret_cast<int32_t &>(field_0x32[0]).
    uint8_t field_0x32[2];
    uint8_t flag_0x34;
    uint8_t flag_0x35;

    uint8_t flag_0x36;
    uint8_t flag_0x37;

    // This 2-byte slot is set whole (formerly flag_word_0x38) in MenuTouchWindow.cpp and also
    // accessed as the named bytes flag_0x38/flag_0x39. Kept as the bytes; the whole-word write is
    // reinterpret_cast<uint16_t &>(flag_0x38).
    uint8_t flag_0x38;
    uint8_t flag_0x39;

    uint8_t field_0x3a;
    uint8_t flag_0x3b;
    uint8_t field_0x3c[0xc];
    uint8_t firstRunPreviewChecked;
};

// OptionsRecord is naturally aligned (no #pragma pack), so its members do not sit at the literal
// hex offsets in their names. These asserts lock the byte layout of the former-union sites
// relative to their anchors so the promoted members cannot shift.
static_assert(sizeof(OptionsRecord) == 0x50, "OptionsRecord size");
static_assert(offsetof(OptionsRecord, flag_0x34) == offsetof(OptionsRecord, field_0x32) + 2, "flag_0x34");
static_assert(offsetof(OptionsRecord, flag_0x35) == offsetof(OptionsRecord, field_0x32) + 3, "flag_0x35");
static_assert(offsetof(OptionsRecord, flag_0x36) == offsetof(OptionsRecord, field_0x32) + 4, "flag_0x36 follows the 4-byte slot");
static_assert(offsetof(OptionsRecord, flag_0x39) == offsetof(OptionsRecord, flag_0x38) + 1, "flag_0x39");
static_assert(offsetof(OptionsRecord, field_0x3a) == offsetof(OptionsRecord, flag_0x38) + 2, "field_0x3a follows the 2-byte slot");

#endif
