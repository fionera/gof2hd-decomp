#ifndef GOF2_CHEATCODE_H
#define GOF2_CHEATCODE_H
#include "gof2/common.h"

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
