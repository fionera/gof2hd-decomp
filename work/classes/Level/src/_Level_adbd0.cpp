#include "class.h"

void operator delete(void *);

// ARM ABI destructors return `this`; model them as functions returning the pointer
// so the result feeds operator delete without a reload.
extern "C" void *dtor_Objective(void *p);
extern "C" void *dtor_BoundingVolume(void *p);
extern "C" void *dtor_StarSystem(void *p);
extern "C" void *dtor_PlayerEgo(void *p);
extern "C" void *dtor_Route(void *p);
extern "C" void *dtor_PSM(void *p);
extern "C" void *dtor_LODManager(void *p);
extern "C" void *dtor_LodMeshMerger(void *p);
extern "C" void *dtor_ArrayKI(void *p);
extern "C" void Level_releaseAEGeometry(void *p);
extern "C" void *dtor_ArrayAEGeometry(void *p);
extern "C" void Level_releaseInt(void *p);
extern "C" void *dtor_ArrayInt(void *p);
extern "C" void Level_releaseAbstractGun(void *p);
extern "C" void *dtor_ArrayAbstractGun(void *p);
extern "C" void Level_releaseKI(void *p);
extern "C" void Level_releaseRadioMessage(void *p);
extern "C" void *dtor_ArrayRadioMessage(void *p);

#define SIMPLE(off, dtor) \
    if (*(void **)((char *)this + (off)) != 0) { \
        operator delete(dtor(*(void **)((char *)this + (off)))); \
    } \
    *(int *)((char *)this + (off)) = 0;

#define ARR(off, release, dtor) \
    if (*(void **)((char *)this + (off)) != 0) { \
        release(*(void **)((char *)this + (off))); \
        if (*(void **)((char *)this + (off)) != 0) { \
            operator delete(dtor(*(void **)((char *)this + (off)))); \
        } \
    } \
    *(int *)((char *)this + (off)) = 0;

Level::~Level() {
    skyboxMesh = -1;
    field_08 = -1;
    skyboxTexture = -1;
    SIMPLE(0x28, dtor_Objective)
    SIMPLE(0x2c, dtor_Objective)
    SIMPLE(0xc4, dtor_BoundingVolume)
    {
        int *p = *(int **)((char *)this + 0xd8);
        if (p != 0) {
            (*(void (**)(int *))(*p + 4))(p);
        }
    }
    *(int *)((char *)this + 0xd8) = 0;
    SIMPLE(0xec, dtor_StarSystem)
    SIMPLE(0xf0, dtor_PlayerEgo)
    SIMPLE(0x180, dtor_Route)
    SIMPLE(0x80, dtor_PSM)
    SIMPLE(0x88, dtor_PSM)
    SIMPLE(0x74, dtor_PSM)
    SIMPLE(0x78, dtor_PSM)
    SIMPLE(0x7c, dtor_PSM)
    SIMPLE(0x90, dtor_PSM)
    SIMPLE(0x84, dtor_PSM)
    SIMPLE(0x98, dtor_PSM)
    SIMPLE(0x9c, dtor_PSM)
    ARR(0xa4, Level_releaseAEGeometry, dtor_ArrayAEGeometry)
    ARR(0xa8, Level_releaseInt, dtor_ArrayInt)
    ARR(0xe4, Level_releaseAbstractGun, dtor_ArrayAbstractGun)
    ARR(0xe8, Level_releaseAbstractGun, dtor_ArrayAbstractGun)
    ARR(0xf8, Level_releaseKI, dtor_ArrayKI)
    ARR(0xfc, Level_releaseKI, dtor_ArrayKI)
    ARR(0xf4, Level_releaseKI, dtor_ArrayKI)
    ARR(0x100, Level_releaseKI, dtor_ArrayKI)
    ARR(0x114, Level_releaseRadioMessage, dtor_ArrayRadioMessage)
    ARR(0x104, Level_releaseAEGeometry, dtor_ArrayAEGeometry)
    SIMPLE(0x0, dtor_LODManager)
    SIMPLE(0xa0, dtor_LodMeshMerger)
    if (*(void **)((char *)this + 0xb0) != 0) {
        operator delete(dtor_ArrayKI(*(void **)((char *)this + 0xb0)));
    }
    *(int *)((char *)this + 0xb0) = 0;
}
