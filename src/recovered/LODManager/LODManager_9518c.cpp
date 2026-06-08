#include "class.h"

// LODManager::LODManager() — real C++ constructor; symbol demangles to contain "LODManager".

extern "C" void *LODManager_operator_new(unsigned n);   // operator new(__SIZE_TYPE__)
extern "C" void *LODManager_Array_ctor(void *p);        // Array<AEGeometry*>::Array(Array*)

struct LODManager {
    void *objects;
    LODManager();
};

LODManager::LODManager()
{
    F<int>(this, 0x04) = 0;
    F<int>(this, 0x08) = 0;
    F<int>(this, 0x0c) = 0;
    void *arr = LODManager_operator_new(0xc);
    LODManager_Array_ctor(arr);
    F<int>(this, 0x10) = 0x3e9;
    this->objects = arr;
}
