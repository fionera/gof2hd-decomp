#ifndef GOF2_GAMEDATA_H
#define GOF2_GAMEDATA_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

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

namespace AbyssEngine { class Engine; }

// Application teardown hook (registered with the engine). Destroys the Globals
// singleton held at GameData offset 0, then the GameData itself.
void OnDestroyApplication(AbyssEngine::Engine* engine);

#endif
