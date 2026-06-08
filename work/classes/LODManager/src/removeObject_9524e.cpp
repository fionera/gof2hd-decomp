#include "class.h"

struct LODArray {
    uint32_t length;
    AEGeometry **data;
};

extern "C" void LODManager_ArrayRemove(AEGeometry *g, LODArray *arr);  // ArrayRemove<AEGeometry*>(AEGeometry*, Array*)

struct LODManager {
    LODArray *objects;
    void removeObject(AEGeometry *g);
};

void LODManager::removeObject(AEGeometry *g)
{
    for (uint32_t i = 0; i < this->objects->length; i++) {
        LODArray *arr = this->objects;
        if (arr->data[i] == g)
            LODManager_ArrayRemove(g, arr);
    }
}
