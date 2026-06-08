#include "class.h"

// LODManager::~LODManager() — real C++ destructor so the demangled symbol contains "~LODManager".

extern "C" void *LODManager_Array_dtor(void *p);   // Array<AEGeometry*>::~Array(Array*)
extern "C" void LODManager_operator_delete(void *p);

struct LODManager {
    void *objects;
    ~LODManager();
};

LODManager::~LODManager()
{
    void *a = *(void **)this;
    if (a != 0)
        LODManager_operator_delete(LODManager_Array_dtor(a));
    *(void **)this = 0;
}
