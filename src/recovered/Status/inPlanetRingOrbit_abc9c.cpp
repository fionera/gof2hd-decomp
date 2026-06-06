#include "class.h"
extern "C" int Station_getIndex(Station *);
uint Status::inPlanetRingOrbit() {
    if (inAlienOrbit() == 0) {
        int idx = Station_getIndex(station);
        unsigned t = idx - 0x78U;
        if (((t >> 1) | (t << 0x1f)) < 7) {
            return 0x69U >> ((idx - 0x78U >> 1) & 0x7f) & 1;
        }
    }
    return 0;
}
