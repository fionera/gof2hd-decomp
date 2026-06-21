#include "engine/render/LODManager.h"
#include "engine/render/AEGeometry.h"
#include "game/core/PaintCanvasClass.h"

#include <cstdlib>

// Android touch-input bridge state (binary .bss). curTouchSize/maxTouchSize are the
// live and allocated touch counts; touches is the base of the Touch buffer (grown by
// realloc). All three are populated by the JNI input layer.
extern "C" int    curTouchSize;
extern "C" int    maxTouchSize;
extern "C" Touch *touches;

// Engine camera/canvas helpers (resolved against the active PaintCanvas).
uint32_t CameraGetCurrent(void *canvas);
Matrix  *CameraGetLocal(void *canvas, uint32_t index);

// Engine singletons; a later externs pass will give these their real types.
extern PaintCanvas **g_LOD_canvas;  // *g_LOD_canvas = PaintCanvas*
extern void  *g_LOD_settings;  // float at +0x28 = LOD distance factor

LODManager::LODManager()
{
    this->cameraPos.x = 0;
    this->cameraPos.y = 0;
    this->cameraPos.z = 0;
    this->timer = 0x3e9;
    this->objects = new Array<AEGeometry*>();
}

LODManager::~LODManager()
{
    delete this->objects;
    this->objects = nullptr;
}

// Register a geometry for LOD tracking, but only if it actually has LOD levels.
void LODManager::addObject(AEGeometry *g)
{
    if (!g->hasLod())
        return;
    ArrayAdd(g, *this->objects);
}

// Erase the matching geometry from the managed object list.
void LODManager::removeObject(AEGeometry *g)
{
    for (uint32_t i = 0; i < this->objects->size(); i++) {
        Array<AEGeometry*> *arr = this->objects;
        if ((*arr)[i] == g)
            ArrayRemove(g, *arr);
    }
}

// Recompute the camera position from the active camera transform and refresh the
// LOD level of every registered geometry.
void LODManager::forceUpdate(int dt, bool useParent)
{
    void *canvas = *g_LOD_canvas;
    float factor = *(float*)((char*)g_LOD_settings + 0x28);

    uint32_t cam = CameraGetCurrent(canvas);
    Matrix  *m   = CameraGetLocal(canvas, cam);
    this->cameraPos = AEMath::MatrixGetPosition(*m);

    for (uint32_t i = 0; i < this->objects->size(); i++) {
        AEGeometry *g = (*this->objects)[i];
        Vector ref = useParent ? g->getParentPosition() : this->cameraPos;
        g->updateLod(ref, factor);
    }
}

// Drive the periodic LOD refresh: accumulate elapsed time and force an update
// once roughly a second has passed.
void LODManager::update(int dt)
{
    this->timer += dt;
    if (this->timer > 1000) {
        this->timer = 0;
        forceUpdate(0, false);
    }
}

// ---------------------------------------------------------------------------
// Android touch-input bridge
// ---------------------------------------------------------------------------

// Append a touch point. Grows the buffer (exact-fit realloc, one slot at a time)
// only when the live count has caught up with the allocated count.
void AddTouch(int x, int y, int id, int action)
{
    int index = curTouchSize;
    curTouchSize = index + 1;

    Touch *buf;
    if (index < maxTouchSize) {
        buf = touches;
    } else {
        maxTouchSize = index + 1;
        buf = static_cast<Touch *>(realloc(touches, (index + 1) * sizeof(Touch)));
        touches = buf;
    }

    buf[curTouchSize - 1].x = x;
    Touch *slot = &buf[curTouchSize - 1];
    slot->y = y;
    slot->id = id;
    slot->action = action;
}

// Current number of live touch points.
int GetTouchCount()
{
    return curTouchSize;
}

// Drop all touch points (the buffer is retained for reuse).
void RemoveTouches()
{
    curTouchSize = 0;
}

// Fetch the touch point at the given index (returned by value).
Touch GetTouch(int index)
{
    return touches[index];
}

// ---------------------------------------------------------------------------
// Native item-information list teardown
// ---------------------------------------------------------------------------

// The 25 native item-information list pointers live as static members of Globals
// (binary .bss 0x228314..); they are not declared in Globals.h, so reach them here
// through their exact mangled symbols. Each holds a malloc'd id buffer (or null).
extern int g_cItemListID_00 asm("_ZN7Globals14cItemListID_00E");
extern int g_cItemListID_01 asm("_ZN7Globals14cItemListID_01E");
extern int g_cItemListID_02 asm("_ZN7Globals14cItemListID_02E");
extern int g_cItemListID_03 asm("_ZN7Globals14cItemListID_03E");
extern int g_cItemListID_04 asm("_ZN7Globals14cItemListID_04E");
extern int g_cItemListID_05 asm("_ZN7Globals14cItemListID_05E");
extern int g_cItemListID_06 asm("_ZN7Globals14cItemListID_06E");
extern int g_cItemListID_07 asm("_ZN7Globals14cItemListID_07E");
extern int g_cItemListID_08 asm("_ZN7Globals14cItemListID_08E");
extern int g_cItemListID_09 asm("_ZN7Globals14cItemListID_09E");
extern int g_cItemListID_10 asm("_ZN7Globals14cItemListID_10E");
extern int g_cItemListID_11 asm("_ZN7Globals14cItemListID_11E");
extern int g_cItemListID_12 asm("_ZN7Globals14cItemListID_12E");
extern int g_cItemListID_13 asm("_ZN7Globals14cItemListID_13E");
extern int g_cItemListID_14 asm("_ZN7Globals14cItemListID_14E");
extern int g_cItemListID_15 asm("_ZN7Globals14cItemListID_15E");
extern int g_cItemListID_16 asm("_ZN7Globals14cItemListID_16E");
extern int g_cItemListID_17 asm("_ZN7Globals14cItemListID_17E");
extern int g_cItemListID_18 asm("_ZN7Globals14cItemListID_18E");
extern int g_cItemListID_19 asm("_ZN7Globals14cItemListID_19E");
extern int g_cItemListID_20 asm("_ZN7Globals14cItemListID_20E");
extern int g_cItemListID_21 asm("_ZN7Globals14cItemListID_21E");
extern int g_cItemListID_22 asm("_ZN7Globals14cItemListID_22E");
extern int g_cItemListID_23 asm("_ZN7Globals14cItemListID_23E");
extern int g_cItemListID_24 asm("_ZN7Globals14cItemListID_24E");

// Release all five native item-information string lists and reset their slots.
// Each list is five item-info id buffers freed all-or-nothing: only when every one
// of the five is live are they released and their slots nulled.
void ndk_resetNativeItemInformationList()
{
    if (g_cItemListID_00 != 0 && g_cItemListID_01 != 0 && g_cItemListID_02 != 0 &&
        g_cItemListID_03 != 0 && g_cItemListID_04 != 0) {
        operator delete[](reinterpret_cast<void *>(g_cItemListID_00));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_01));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_02));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_03));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_04));
        g_cItemListID_01 = 0;
        g_cItemListID_00 = 0;
        g_cItemListID_02 = 0;
        g_cItemListID_03 = 0;
        g_cItemListID_04 = 0;
    }
    if (g_cItemListID_05 != 0 && g_cItemListID_06 != 0 && g_cItemListID_07 != 0 &&
        g_cItemListID_08 != 0 && g_cItemListID_09 != 0) {
        operator delete[](reinterpret_cast<void *>(g_cItemListID_05));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_06));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_07));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_08));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_09));
        g_cItemListID_06 = 0;
        g_cItemListID_05 = 0;
        g_cItemListID_07 = 0;
        g_cItemListID_08 = 0;
        g_cItemListID_09 = 0;
    }
    if (g_cItemListID_10 != 0 && g_cItemListID_11 != 0 && g_cItemListID_12 != 0 &&
        g_cItemListID_13 != 0 && g_cItemListID_14 != 0) {
        operator delete[](reinterpret_cast<void *>(g_cItemListID_10));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_11));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_12));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_13));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_14));
        g_cItemListID_11 = 0;
        g_cItemListID_10 = 0;
        g_cItemListID_12 = 0;
        g_cItemListID_13 = 0;
        g_cItemListID_14 = 0;
    }
    if (g_cItemListID_15 != 0 && g_cItemListID_16 != 0 && g_cItemListID_17 != 0 &&
        g_cItemListID_18 != 0 && g_cItemListID_19 != 0) {
        operator delete[](reinterpret_cast<void *>(g_cItemListID_15));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_16));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_17));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_18));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_19));
        g_cItemListID_16 = 0;
        g_cItemListID_15 = 0;
        g_cItemListID_17 = 0;
        g_cItemListID_18 = 0;
        g_cItemListID_19 = 0;
    }
    if (g_cItemListID_20 != 0 && g_cItemListID_21 != 0 && g_cItemListID_22 != 0 &&
        g_cItemListID_23 != 0 && g_cItemListID_24 != 0) {
        operator delete[](reinterpret_cast<void *>(g_cItemListID_20));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_21));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_22));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_23));
        operator delete[](reinterpret_cast<void *>(g_cItemListID_24));
        g_cItemListID_21 = 0;
        g_cItemListID_20 = 0;
        g_cItemListID_22 = 0;
        g_cItemListID_23 = 0;
        g_cItemListID_24 = 0;
    }
}
