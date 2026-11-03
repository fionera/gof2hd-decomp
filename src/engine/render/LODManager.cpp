#include "engine/render/LODManager.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"

#include <cstdlib>

// Defined in NdkAutosave.cpp: the original first mentions these globals before
// Globals::Canvas, which is first referenced in this TU.  The initializer stays
// here so the GOT first-reference order (curTouchSize, maxTouchSize, touches,
// Canvas) is unchanged.
extern Touch *touches;
extern int maxTouchSize;
extern int curTouchSize;

namespace {
struct TouchBufferInit {
    TouchBufferInit() { touches = static_cast<Touch *>(realloc(touches, sizeof(Touch))); }
};
TouchBufferInit touchBufferInit;
}

LODManager::LODManager() {
    this->cameraPos.x = 0;
    this->cameraPos.y = 0;
    this->cameraPos.z = 0;
    this->objects = new Array<AEGeometry *>();
    this->timer = 1001;
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
        if ((*this->objects)[i] == g)
            ArrayRemove(g, *this->objects);
    }
}

void LODManager::forceUpdate(int dt, bool useParent) {
    // Globals::options is a byte array; the LOD distance factor is the float at offset 0x28.
    float factor = reinterpret_cast<const float &>(Globals::options[0x28]);
    // CameraGetLocal exposes the raw float storage of the current camera matrix.
    this->cameraPos = AbyssEngine::AEMath::MatrixGetPosition(
            *reinterpret_cast<const Matrix *>(Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent())));

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
