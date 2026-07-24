#ifndef GOF2_GAMEDATA_H
#define GOF2_GAMEDATA_H
#include "engine/core/Array.h"
#include "AEString.h"
class Globals;

namespace AbyssEngine {
    class Engine;
}

class GameData {
public:
    Globals *globals;
    uint16_t field_0x04;
    int32_t field_0x08;
    uint16_t field_0x0c;
    uint8_t field_0x0e;
    int32_t field_0x10;
    uint16_t field_0x14;
    AbyssEngine::String field_0x18;
    AbyssEngine::String field_0x24;
    AbyssEngine::String field_0x30;

    int32_t field_0x3c;
    int32_t field_0x40;
    uint8_t field_0x44;
    int32_t field_0x48;
    uint8_t field_0x4c;
    int32_t field_0x50;
    int32_t field_0x54;
    int32_t field_0x58;
    int32_t field_0x5c;
    int32_t field_0x60;
    int32_t field_0x64;
    int32_t field_0x68;
    // 0x6c..0x77 are byte fields: the original writes this run at unaligned offsets
    // (str.w [+0x6d], str.w [+0x71], strh.w [+0x75], strb.w [+0x77]) — merged byte stores,
    // impossible for naturally-aligned wider members.
    uint8_t field_0x6c;
    uint8_t field_0x6d;
    uint8_t field_0x6e;
    uint8_t field_0x6f;
    uint8_t field_0x70;
    uint8_t field_0x71;
    uint8_t field_0x72;
    uint8_t field_0x73;
    uint8_t field_0x74;
    uint8_t field_0x75;
    uint8_t field_0x76;
    uint8_t field_0x77;
    uint16_t field_0x78;
    uint8_t field_0x7a;
    AbyssEngine::String field_0x7c;
    AbyssEngine::String field_0x88;
    AbyssEngine::String field_0x94;
    int32_t field_0xa0;
    uint8_t field_0xa4;
    uint8_t field_0xa5;
    AbyssEngine::String field_0xa8;
    uint8_t field_0xb4;
    AbyssEngine::String field_0xb8;
    uint8_t field_0xc4;

    GameData() {
    }

    ~GameData() {
    }
};

void OnDestroyApplication(AbyssEngine::Engine *engine);

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(GameData, field_0x6c) == 0x6c, "GameData::field_0x6c offset");
static_assert(__builtin_offsetof(GameData, field_0x6d) == 0x6d, "GameData::field_0x6d offset");
static_assert(__builtin_offsetof(GameData, field_0x71) == 0x71, "GameData::field_0x71 offset");
static_assert(__builtin_offsetof(GameData, field_0x75) == 0x75, "GameData::field_0x75 offset");
static_assert(__builtin_offsetof(GameData, field_0x78) == 0x78, "GameData::field_0x78 offset");
static_assert(__builtin_offsetof(GameData, field_0x7c) == 0x7c, "GameData::field_0x7c offset");
static_assert(__builtin_offsetof(GameData, field_0xa0) == 0xa0, "GameData::field_0xa0 offset");
static_assert(__builtin_offsetof(GameData, field_0xb8) == 0xb8, "GameData::field_0xb8 offset");
static_assert(__builtin_offsetof(GameData, field_0xc4) == 0xc4, "GameData::field_0xc4 offset");
static_assert(sizeof(GameData) == 0xc8, "GameData size");
#endif

#endif
