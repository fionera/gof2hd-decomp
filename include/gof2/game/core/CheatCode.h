#ifndef GOF2_CHEATCODE_H
#define GOF2_CHEATCODE_H
#include "gof2/common.h"
// AbyssEngine::CheatCode — layout recovered from Ghidra (0x84378 ctor):
//   +0x0 keys  (Array<unsigned short>*),  +0x4 value (int),  +0x8 pos (int)
namespace AbyssEngine {

class CheatCode {
public:
    Array<uint16_t> *keys;   // +0x0 sequence of key codes
    int              value;  // +0x4 reward/value associated with the code
    int              pos;    // +0x8 current match position

    CheatCode();
    ~CheatCode();
    bool Update(uint16_t key);
};

} // namespace AbyssEngine

#endif
