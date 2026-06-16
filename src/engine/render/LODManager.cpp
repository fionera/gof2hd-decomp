#include "engine/render/LODManager.h"
#include "engine/render/AEGeometry.h"

// Engine camera/canvas helpers (resolved against the active PaintCanvas).
uint32_t CameraGetCurrent(void *canvas);
Matrix  *CameraGetLocal(void *canvas, uint32_t index);

// Engine singletons; a later externs pass will give these their real types.
extern void **g_LOD_canvas;    // *g_LOD_canvas = PaintCanvas*
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
            arr->erase(arr->begin() + i);
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
