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

    // A 2-byte field at the ODD offset 0xd. A uint16_t cannot sit at an odd offset under natural
    // alignment (it would be bumped to 0xe, shifting every following member off its named offset and
    // re-padding at flag_word_0x1c/flag_dword_0x20). Kept as the two raw bytes so it lands at 0xd; the
    // whole-word write is reinterpret_cast<uint16_t &>(flag_word_0xd[0]) (see MenuTouchWindow.cpp).
    uint8_t flag_word_0xd[2];

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
    // The original struct is 0x50 bytes (Ghidra). firstRunPreviewChecked sits at 0x48; the trailing
    // 0x49..0x4f bytes are reserved padding that keeps sizeof(OptionsRecord) == 0x50.
    uint8_t _pad_0x49[7];
};

// Every named field now lands at the literal hex offset in its name (the struct byte-packs with
// alignment 4: all multi-byte members sit at naturally-aligned offsets except the 2-byte field at
// the odd offset 0xd, which is modeled as raw bytes). These absolute asserts lock that layout so no
// member can drift again.
#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(OptionsRecord) == 0x50, "OptionsRecord size");
static_assert(offsetof(OptionsRecord, flag_word_0xd) == 0xd, "flag_word_0xd");
static_assert(offsetof(OptionsRecord, flag_0xf) == 0xf, "flag_0xf");
static_assert(offsetof(OptionsRecord, field_0x10) == 0x10, "field_0x10");
static_assert(offsetof(OptionsRecord, flag_0x13) == 0x13, "flag_0x13");
static_assert(offsetof(OptionsRecord, field_0x14) == 0x14, "field_0x14");
static_assert(offsetof(OptionsRecord, flag_0x15) == 0x15, "flag_0x15");
static_assert(offsetof(OptionsRecord, field_0x16) == 0x16, "field_0x16");
static_assert(offsetof(OptionsRecord, flag_0x17) == 0x17, "flag_0x17");
static_assert(offsetof(OptionsRecord, field_0x18) == 0x18, "field_0x18");
static_assert(offsetof(OptionsRecord, flag_word_0x1c) == 0x1c, "flag_word_0x1c");
static_assert(offsetof(OptionsRecord, flag_0x1e) == 0x1e, "flag_0x1e");
static_assert(offsetof(OptionsRecord, field_0x1f) == 0x1f, "field_0x1f");
static_assert(offsetof(OptionsRecord, flag_dword_0x20) == 0x20, "flag_dword_0x20");
static_assert(offsetof(OptionsRecord, flag_0x24) == 0x24, "flag_0x24");
static_assert(offsetof(OptionsRecord, field_0x25) == 0x25, "field_0x25");
static_assert(offsetof(OptionsRecord, flag_0x26) == 0x26, "flag_0x26");
static_assert(offsetof(OptionsRecord, field_0x27) == 0x27, "field_0x27");
static_assert(offsetof(OptionsRecord, fadeValue) == 0x2c, "fadeValue (next named field)");
static_assert(offsetof(OptionsRecord, field_0x30) == 0x30, "field_0x30");
static_assert(offsetof(OptionsRecord, flag_0x31) == 0x31, "flag_0x31");
static_assert(offsetof(OptionsRecord, field_0x32) == 0x32, "field_0x32");
static_assert(offsetof(OptionsRecord, flag_0x34) == 0x34, "flag_0x34");
static_assert(offsetof(OptionsRecord, flag_0x35) == 0x35, "flag_0x35");
static_assert(offsetof(OptionsRecord, flag_0x36) == 0x36, "flag_0x36");
static_assert(offsetof(OptionsRecord, flag_0x37) == 0x37, "flag_0x37");
static_assert(offsetof(OptionsRecord, flag_0x38) == 0x38, "flag_0x38");
static_assert(offsetof(OptionsRecord, flag_0x39) == 0x39, "flag_0x39");
static_assert(offsetof(OptionsRecord, field_0x3a) == 0x3a, "field_0x3a");
static_assert(offsetof(OptionsRecord, flag_0x3b) == 0x3b, "flag_0x3b");
static_assert(offsetof(OptionsRecord, field_0x3c) == 0x3c, "field_0x3c");
static_assert(offsetof(OptionsRecord, firstRunPreviewChecked) == 0x48, "firstRunPreviewChecked (next named field)");
#endif

#endif
