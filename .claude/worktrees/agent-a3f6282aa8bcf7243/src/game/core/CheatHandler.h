#ifndef GOF2_CHEATHANDLER_H
#define GOF2_CHEATHANDLER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {

// ArrayAdd / ArrayRemoveAll are the engine's container free-functions, declared once in common.h
// (global scope). Re-declaring them here in namespace AbyssEngine created a second overload that
// made every call ambiguous, so we rely on the common.h definitions.

struct CheatCode {
    Array<uint16_t> *keys;
    int32_t value;
    uint32_t pos;

    CheatCode();
    ~CheatCode();

    bool Update(uint16_t key);
};

// Maps a digit '0'..'9' to a hardware key code.
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

// Cheat-code activation callback registered with the ApplicationManager by
// OnCreateApplication (CheatFunc: code = the matched cheat's value, data = context).
// Applies the unlocked effect for the entered code; its body lives in its own TU.
void OnCheatActivated(int code, void *data);

#endif
