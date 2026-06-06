#include "class.h"
extern "C" int Station_getIndex(Station *);
bool Status::inDeepScienceOrbit() {
    if (inAlienOrbit()) return false;
    if (Station_getIndex(station) == 10) return true;
    return Station_getIndex(station) == 100;
}
