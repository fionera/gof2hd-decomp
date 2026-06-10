#ifndef GOF2_CHEATHANDLER_H
#define GOF2_CHEATHANDLER_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct CheatHandler {
    CheatFunc func;             // 0x00
    void *context;              // 0x04
    Array<CheatCode *> *codes;  // 0x08
    KeyCode *keys;              // 0x0c

    CheatHandler(KeyCode *keys);
    ~CheatHandler();

    void AddCheatCode(const String &code, int value);
    void SetCheatFunc(CheatFunc func, void *context);
    void Update(uint16_t key);
};
#endif
