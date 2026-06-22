#ifndef GOF2_CHEATHANDLER_H
#define GOF2_CHEATHANDLER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {
    struct CheatCode {
        Array<uint16_t> *keys;
        int32_t value;
        uint32_t pos;

        CheatCode();

        ~CheatCode();

        bool Update(uint16_t key);
    };

    class KeyCode;

    typedef void (*CheatFunc)(int, void *);

    class CheatHandler {
    public:
        CheatFunc func;
        void *context;
        Array<CheatCode *> *codes;
        KeyCode *keys;

        CheatHandler(KeyCode *keys);

        ~CheatHandler();

        void AddCheatCode(const String &code, int value);

        void SetCheatFunc(CheatFunc func, void *context);

        void Update(uint16_t key);
    };
} // namespace AbyssEngine

void OnCheatActivated(int code, void *data);

#endif
