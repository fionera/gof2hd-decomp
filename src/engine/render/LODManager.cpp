#include "engine/render/LODManager.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

#include <cstdlib>

int curTouchSize = 0;
int maxTouchSize = 0;
// The original allocates a one-entry buffer at static-init time (its second .init_array entry:
// touches = realloc(touches, 16)).
Touch *touches = static_cast<Touch *>(realloc(touches, sizeof(Touch)));



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
        Globals::cItemListID_00 = nullptr;
        Globals::cItemListID_01 = nullptr;
        Globals::cItemListID_02 = nullptr;
        Globals::cItemListID_03 = nullptr;
        Globals::cItemListID_04 = nullptr;
    }
    if (Globals::cItemListName_00 != nullptr && Globals::cItemListName_01 != nullptr &&
        Globals::cItemListName_02 != nullptr && Globals::cItemListName_03 != nullptr &&
        Globals::cItemListName_04 != nullptr) {
        operator delete[](Globals::cItemListName_00);
        operator delete[](Globals::cItemListName_01);
        operator delete[](Globals::cItemListName_02);
        operator delete[](Globals::cItemListName_03);
        operator delete[](Globals::cItemListName_04);
        Globals::cItemListName_00 = nullptr;
        Globals::cItemListName_01 = nullptr;
        Globals::cItemListName_02 = nullptr;
        Globals::cItemListName_03 = nullptr;
        Globals::cItemListName_04 = nullptr;
    }
    if (Globals::cItemListDescription_00 != nullptr && Globals::cItemListDescription_01 != nullptr &&
        Globals::cItemListDescription_02 != nullptr && Globals::cItemListDescription_03 != nullptr &&
        Globals::cItemListDescription_04 != nullptr) {
        operator delete[](Globals::cItemListDescription_00);
        operator delete[](Globals::cItemListDescription_01);
        operator delete[](Globals::cItemListDescription_02);
        operator delete[](Globals::cItemListDescription_03);
        operator delete[](Globals::cItemListDescription_04);
        Globals::cItemListDescription_00 = nullptr;
        Globals::cItemListDescription_01 = nullptr;
        Globals::cItemListDescription_02 = nullptr;
        Globals::cItemListDescription_03 = nullptr;
        Globals::cItemListDescription_04 = nullptr;
    }
    if (Globals::cItemListCurrency_00 != nullptr && Globals::cItemListCurrency_01 != nullptr &&
        Globals::cItemListCurrency_02 != nullptr && Globals::cItemListCurrency_03 != nullptr &&
        Globals::cItemListCurrency_04 != nullptr) {
        operator delete[](Globals::cItemListCurrency_00);
        operator delete[](Globals::cItemListCurrency_01);
        operator delete[](Globals::cItemListCurrency_02);
        operator delete[](Globals::cItemListCurrency_03);
        operator delete[](Globals::cItemListCurrency_04);
        Globals::cItemListCurrency_00 = nullptr;
        Globals::cItemListCurrency_01 = nullptr;
        Globals::cItemListCurrency_02 = nullptr;
        Globals::cItemListCurrency_03 = nullptr;
        Globals::cItemListCurrency_04 = nullptr;
    }
    if (Globals::cItemListPrice_00 != nullptr && Globals::cItemListPrice_01 != nullptr &&
        Globals::cItemListPrice_02 != nullptr && Globals::cItemListPrice_03 != nullptr &&
        Globals::cItemListPrice_04 != nullptr) {
        operator delete[](Globals::cItemListPrice_00);
        operator delete[](Globals::cItemListPrice_01);
        operator delete[](Globals::cItemListPrice_02);
        operator delete[](Globals::cItemListPrice_03);
        operator delete[](Globals::cItemListPrice_04);
        Globals::cItemListPrice_00 = nullptr;
        Globals::cItemListPrice_01 = nullptr;
        Globals::cItemListPrice_02 = nullptr;
        Globals::cItemListPrice_03 = nullptr;
        Globals::cItemListPrice_04 = nullptr;
    }
}

LODManager::LODManager() {
    this->cameraPos.x = 0;
    this->cameraPos.y = 0;
    this->cameraPos.z = 0;
    this->objects = new Array<AEGeometry *>();
    this->timer = 0x3e9;
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
    // Options block: the LOD distance factor is the float at offset 0x28.
    float factor = *reinterpret_cast<float *>(&Globals::options[0x28]);
    PaintCanvas *canvas = Globals::Canvas;
    this->cameraPos = AbyssEngine::AEMath::MatrixGetPosition(
            *reinterpret_cast<const Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent())));

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
