#ifndef GOF2_GAMEDATA_H
#define GOF2_GAMEDATA_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- GameData (top-level engine class, not namespaced). This is the
// opaque "application data" object the engine stashes through
// ApplicationManager::SetApplicationData; OnCreateApplication() allocates and seeds it.
//
// It owns the game-wide Globals singleton (offset 0) and eight embedded
// AbyssEngine::String members; construction/destruction of those strings is handled by
// the implicit member init/teardown. The remaining members are the per-session game
// state (selected slots, transition-request flags, score bookkeeping and the cached
// asset-path / DLC strings) the menu modules drive each frame.
//
// Offsets in the trailing comments are the field offsets in the shipped 0xC8-byte
// binary layout; the recovered struct keeps the same field *order* and names, but the
// real std::u16string-backed String members make the C++ layout larger than the
// original (this is a linked-exact port, not a byte-exact one).
class Globals;

class GameData {
public:
    Globals*            globals;          // +0x00 game-wide Globals singleton (owned)
    uint16_t            field_0x04;       // +0x04
    int32_t             field_0x08;       // +0x08
    uint16_t            field_0x0c;       // +0x0c
    uint8_t             field_0x0e;       // +0x0e
    int32_t             field_0x10;       // +0x10 (seeded to -1)
    uint16_t            field_0x14;       // +0x14
    AbyssEngine::String field_0x18;       // +0x18
    AbyssEngine::String field_0x24;       // +0x24
    AbyssEngine::String field_0x30;       // +0x30
    // +0x3c..0x3f: per-frame screen-transition request flags packed into one word
    // (the ctor clears it; ndk23_newrender polls/clears the individual bytes at
    // appData[0x3c..0x3f] and promotes 0x3d into 0x40).
    int32_t             field_0x3c;       // +0x3c transition-request flag word
    int32_t             field_0x40;       // +0x40 (set when the 0x3d flag fires)
    uint8_t             field_0x44;       // +0x44
    int32_t             field_0x48;       // +0x48 (seeded to -1)
    uint8_t             field_0x4c;       // +0x4c (seeded to 1)
    int32_t             field_0x50;       // +0x50
    int32_t             field_0x54;       // +0x54
    int32_t             field_0x58;       // +0x58
    int32_t             field_0x5c;       // +0x5c
    int32_t             field_0x60;       // +0x60
    int32_t             field_0x64;       // +0x64
    int32_t             field_0x68;       // +0x68
    int32_t             field_0x6c;       // +0x6c
    int32_t             field_0x6d;       // +0x6d (unaligned in the original)
    int32_t             field_0x71;       // +0x71 (unaligned in the original)
    uint16_t            field_0x75;       // +0x75
    uint8_t             field_0x77;       // +0x77
    uint16_t            field_0x78;       // +0x78
    uint8_t             field_0x7a;       // +0x7a
    AbyssEngine::String field_0x7c;       // +0x7c
    AbyssEngine::String field_0x88;       // +0x88
    AbyssEngine::String field_0x94;       // +0x94
    int32_t             field_0xa0;       // +0xa0
    uint8_t             field_0xa4;       // +0xa4
    uint8_t             field_0xa5;       // +0xa5
    AbyssEngine::String field_0xa8;       // +0xa8 (asset / DLC path string)
    uint8_t             field_0xb4;       // +0xb4
    AbyssEngine::String field_0xb8;       // +0xb8 (asset / DLC path string)
    uint8_t             field_0xc4;       // +0xc4

    GameData();
    ~GameData();
};

namespace AbyssEngine { class Engine; }

// Application teardown hook (registered with the engine). Destroys the Globals
// singleton held at GameData offset 0, then the GameData itself.
void OnDestroyApplication(AbyssEngine::Engine* engine);

#endif
