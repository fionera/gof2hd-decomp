#ifndef GOF2_CHEATCODE_H
#define GOF2_CHEATCODE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

namespace AbyssEngine {

class CheatCode {
public:
    Array<uint16_t> *keys;   // sequence of key codes to match
    int              value;  // reward/value associated with the code
    int              pos;    // current match position

    CheatCode();
    ~CheatCode();
    bool Update(uint16_t key);
};

} // namespace AbyssEngine

#endif
