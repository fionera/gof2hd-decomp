#include "gof2/game/core/CheatCode.h"




// ---- CheatCode_74378.cpp ----
namespace AbyssEngine {

CheatCode::CheatCode()
{
    keys = new Array<uint16_t>();
    value = 0;
    pos = 0;
}

} // namespace AbyssEngine

// ---- Update_743f0.cpp ----
namespace AbyssEngine {

bool CheatCode::Update(uint16_t key)
{
    Array<uint16_t> *a = keys;
    uint32_t p = pos;
    uint16_t *d = a->data();
    uint32_t n = a->size();
    if (p >= n) {
        return false;
    }
    if (d[p] != key) {
        pos = 0;
        bool first = (d[0] == key);
        uint32_t one = (uint32_t)first;
        pos = (first && n != one);
        return n == one;
    }
    p = p + 1;
    uint32_t np = p;
    if (p == n) {
        np = 0;
    }
    pos = np;
    return p == n;
}

} // namespace AbyssEngine

// ---- _CheatCode_743c0.cpp ----
namespace AbyssEngine {

CheatCode::~CheatCode()
{
    if (keys) {
        delete keys;
    }
    keys = 0;
}

} // namespace AbyssEngine
