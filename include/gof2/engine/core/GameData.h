#ifndef GOF2_GAMEDATA_H
#define GOF2_GAMEDATA_H
#include "gof2/common.h"

// Galaxy on Fire 2 -- GameData (top-level engine class, not namespaced).
// Owns eight AbyssEngine::String members; construction and destruction of those
// strings are handled by the implicit member init/teardown.
class GameData {
public:
    AbyssEngine::String field_0x18;
    AbyssEngine::String field_0x24;
    AbyssEngine::String field_0x30;
    AbyssEngine::String field_0x7c;
    AbyssEngine::String field_0x88;
    AbyssEngine::String field_0x94;
    AbyssEngine::String field_0xa8;
    AbyssEngine::String field_0xb8;

    GameData();
    ~GameData();
};
#endif
