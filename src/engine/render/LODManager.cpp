#include "engine/render/LODManager.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"

#include <cstdlib>

extern "C" int curTouchSize;
extern "C" int maxTouchSize;
extern "C" Touch *touches;

uint32_t CameraGetCurrent(void *canvas);

Matrix *CameraGetLocal(void *canvas, uint32_t index);

extern PaintCanvas **g_LOD_canvas;
extern void *g_LOD_settings;

LODManager::LODManager() {
    this->cameraPos.x = 0;
    this->cameraPos.y = 0;
    this->cameraPos.z = 0;
    this->timer = 0x3e9;
    this->objects = new Array<AEGeometry *>();
}

LODManager::~LODManager() {
    delete this->objects;
    this->objects = nullptr;
}

void LODManager::addObject(AEGeometry *g) {
    if (!g->hasLod())
        return;
    ArrayAdd(g, *this->objects);
}

void LODManager::removeObject(AEGeometry *g) {
    for (uint32_t i = 0; i < this->objects->size(); i++) {
        Array<AEGeometry *> *arr = this->objects;
        if ((*arr)[i] == g)
            ArrayRemove(g, *arr);
    }
}

void LODManager::forceUpdate(int dt, bool useParent) {
    void *canvas = *g_LOD_canvas;
    float factor = *(float *) ((char *) g_LOD_settings + 0x28);

    uint32_t cam = CameraGetCurrent(canvas);
    Matrix *m = CameraGetLocal(canvas, cam);
    this->cameraPos = AbyssEngine::AEMath::MatrixGetPosition(*m);

    for (uint32_t i = 0; i < this->objects->size(); i++) {
        AEGeometry *g = (*this->objects)[i];
        Vector ref = useParent ? g->getParentPosition() : this->cameraPos;
        g->updateLod(ref, factor);
    }
}

void LODManager::update(int dt) {
    this->timer += dt;
    if (this->timer > 1000) {
        this->timer = 0;
        forceUpdate(0, false);
    }
}

void AddTouch(int x, int y, int id, int action) {
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

int GetTouchCount() {
    return curTouchSize;
}

void RemoveTouches() {
    curTouchSize = 0;
}

Touch GetTouch(int index) {
    return touches[index];
}

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

extern "C" void ndk_resetNativeItemInformationList() {
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
