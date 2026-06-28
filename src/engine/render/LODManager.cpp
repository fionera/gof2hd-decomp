#include "engine/render/LODManager.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

#include <cstdlib>

int curTouchSize = 0;
int maxTouchSize = 0;
Touch *touches = nullptr;

uint32_t CameraGetCurrent(void *canvas); // lint: void_ptr (free-function signature; retype changes mangling)

Matrix *CameraGetLocal(void *canvas, uint32_t index);

// lint: void_ptr (free-function signature; retype changes mangling)

static PaintCanvas **g_LOD_canvas = nullptr;

static char *cItemListID_05 = nullptr, *cItemListID_06 = nullptr, *cItemListID_07 = nullptr,
        *cItemListID_08 = nullptr, *cItemListID_09 = nullptr, *cItemListID_10 = nullptr,
        *cItemListID_11 = nullptr, *cItemListID_12 = nullptr, *cItemListID_13 = nullptr,
        *cItemListID_14 = nullptr, *cItemListID_15 = nullptr, *cItemListID_16 = nullptr,
        *cItemListID_17 = nullptr, *cItemListID_18 = nullptr, *cItemListID_19 = nullptr,
        *cItemListID_20 = nullptr, *cItemListID_21 = nullptr, *cItemListID_22 = nullptr,
        *cItemListID_23 = nullptr, *cItemListID_24 = nullptr;

struct LODSettings {
    unsigned char reserved_0x00[0x28];
    float distanceFactor;
};
#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(LODSettings, distanceFactor) == 0x28,
              "LODSettings::distanceFactor must sit at offset 0x28");
#endif

static LODSettings *g_LOD_settings = nullptr;

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
    PaintCanvas *canvas = *g_LOD_canvas;
    float factor = g_LOD_settings->distanceFactor;

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

extern "C" void ndk_resetNativeItemInformationList() { // lint: extern_c (native ABI boundary; original exports the symbol unmangled / GL+libc C ABI)
    if (Globals::cItemListID_00 != nullptr && Globals::cItemListID_01 != nullptr &&
        Globals::cItemListID_02 != nullptr && Globals::cItemListID_03 != nullptr &&
        Globals::cItemListID_04 != nullptr) {
        operator delete[](Globals::cItemListID_00);
        operator delete[](Globals::cItemListID_01);
        operator delete[](Globals::cItemListID_02);
        operator delete[](Globals::cItemListID_03);
        operator delete[](Globals::cItemListID_04);
        Globals::cItemListID_01 = nullptr;
        Globals::cItemListID_00 = nullptr;
        Globals::cItemListID_02 = nullptr;
        Globals::cItemListID_03 = nullptr;
        Globals::cItemListID_04 = nullptr;
    }
    if (cItemListID_05 != nullptr && cItemListID_06 != nullptr &&
        cItemListID_07 != nullptr && cItemListID_08 != nullptr &&
        cItemListID_09 != nullptr) {
        operator delete[](cItemListID_05);
        operator delete[](cItemListID_06);
        operator delete[](cItemListID_07);
        operator delete[](cItemListID_08);
        operator delete[](cItemListID_09);
        cItemListID_06 = nullptr;
        cItemListID_05 = nullptr;
        cItemListID_07 = nullptr;
        cItemListID_08 = nullptr;
        cItemListID_09 = nullptr;
    }
    if (cItemListID_10 != nullptr && cItemListID_11 != nullptr &&
        cItemListID_12 != nullptr && cItemListID_13 != nullptr &&
        cItemListID_14 != nullptr) {
        operator delete[](cItemListID_10);
        operator delete[](cItemListID_11);
        operator delete[](cItemListID_12);
        operator delete[](cItemListID_13);
        operator delete[](cItemListID_14);
        cItemListID_11 = nullptr;
        cItemListID_10 = nullptr;
        cItemListID_12 = nullptr;
        cItemListID_13 = nullptr;
        cItemListID_14 = nullptr;
    }
    if (cItemListID_15 != nullptr && cItemListID_16 != nullptr &&
        cItemListID_17 != nullptr && cItemListID_18 != nullptr &&
        cItemListID_19 != nullptr) {
        operator delete[](cItemListID_15);
        operator delete[](cItemListID_16);
        operator delete[](cItemListID_17);
        operator delete[](cItemListID_18);
        operator delete[](cItemListID_19);
        cItemListID_16 = nullptr;
        cItemListID_15 = nullptr;
        cItemListID_17 = nullptr;
        cItemListID_18 = nullptr;
        cItemListID_19 = nullptr;
    }
    if (cItemListID_20 != nullptr && cItemListID_21 != nullptr &&
        cItemListID_22 != nullptr && cItemListID_23 != nullptr &&
        cItemListID_24 != nullptr) {
        operator delete[](cItemListID_20);
        operator delete[](cItemListID_21);
        operator delete[](cItemListID_22);
        operator delete[](cItemListID_23);
        operator delete[](cItemListID_24);
        cItemListID_21 = nullptr;
        cItemListID_20 = nullptr;
        cItemListID_22 = nullptr;
        cItemListID_23 = nullptr;
        cItemListID_24 = nullptr;
    }
}
