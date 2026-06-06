#include "class.h"
extern "C" int Station_getIndex(Station *);
bool Status::inSupernovaOrbit() {
    if (inAlienOrbit()) return false;
    return Station_getIndex(station) == 0x6d;
}
