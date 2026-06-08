#include "class.h"

int SystemPathFinder::getJumpDistance(Array<SolarSystem *> *systems, int from,
                                      int to) {
    Array<int> *path = getSystemPath(systems, from, to);
    if (path != 0) {
        int length = path->length;
        ArrayRelease<int>(*path);
        path->~Array<int>();
        operator delete(path);
        return length - 1;
    }
    return 0;
}
