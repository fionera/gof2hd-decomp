#ifndef GOF2_CHEATHANDLER_H
#define GOF2_CHEATHANDLER_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/core/CheatCode.h"

namespace AbyssEngine {
    class KeyCode;
}

namespace AbyssEngine {
    typedef void (*CheatFunc)(int, void *); // lint: void_ptr (CheatFunc ABI baked into SetCheatFunc/CheatSetCallback mangling)

    // lint: void_ptr (CheatFunc ABI baked into SetCheatFunc/CheatSetCallback mangling)

    class CheatHandler {
    public:
        CheatFunc func;
        void *context; // lint: void_ptr (opaque callback user-data passed to CheatFunc's void* parameter)
        Array<CheatCode *> *codes;
        KeyCode *keys;

        CheatHandler(KeyCode *keys);

        ~CheatHandler();

        void AddCheatCode(const String &code, int value);

        void SetCheatFunc(CheatFunc func, void *context); // lint: void_ptr (exported method signature)

        void Update(uint16_t key);
    };
}

void OnCheatActivated(int code, void *data); // lint: void_ptr (exported free function signature)

#endif
