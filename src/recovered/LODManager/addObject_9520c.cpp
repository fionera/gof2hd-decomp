#include "class.h"

extern "C" int LODManager_hasLod(AEGeometry *g);              // AEGeometry::hasLod()
extern "C" void LODManager_addObject_tail(AEGeometry *g, void *objects);

struct LODManager {
    void *objects;
    void addObject(AEGeometry *g);
};

void LODManager::addObject(AEGeometry *g)
{
    if (LODManager_hasLod(g) == 0)
        return;
    return LODManager_addObject_tail(g, this->objects);
}
