#include "class.h"
uint Status::orbitHasPlanetRing(int index) {
    unsigned t = index - 0x78U;
    if (((t >> 1) | (t << 0x1f)) < 7) {
        return 0x69U >> ((index - 0x78U >> 1) & 0x7f) & 1;
    }
    return 0;
}
