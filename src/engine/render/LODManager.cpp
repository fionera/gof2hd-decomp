#include "gof2/engine/render/LODManager.h"
#include "gof2/engine/render/AEGeometry.h"

uint32_t CameraGetCurrent(void *canvas);
Matrix *CameraGetLocal(void *canvas, uint32_t index);

// ---- addObject_9520c.cpp ----
extern "C" int LODManager_hasLod(AEGeometry *g);              // AEGeometry::hasLod()

void LODManager::addObject(AEGeometry *g)
{
    if (LODManager_hasLod(g) == 0)
        return;
    return this->addObject_tail(g, this->objects);
}

// ---- addObject_tail ----------------------------------------------------------
// The decompiler peeled addObject()'s trailing tail-call (a GOT veneer) into its
// own thunk. It is ArrayAdd<AEGeometry*>(g, objects): append the geometry to the
// managed object list.
void LODManager::addObject_tail(AEGeometry *g, Array<AEGeometry*> *objects)
{
    ArrayAdd(g, *objects);
}

// ---- LODManager_9518c.cpp ----
// LODManager::LODManager() — real C++ constructor; symbol demangles to contain "LODManager".

LODManager::LODManager()
{
    this->cameraPos.x = 0;
    this->cameraPos.y = 0;
    this->cameraPos.z = 0;
    this->timer = 0x3e9;
    this->objects = new Array<AEGeometry*>();
}

// ---- _LODManager_951dc.cpp ----
// LODManager::~LODManager() — real C++ destructor so the demangled symbol contains "~LODManager".

LODManager::~LODManager()
{
    if (this->objects != 0)
        delete this->objects;
    this->objects = 0;
}

// ---- removeObject_9524e.cpp ----
extern "C" void LODManager_ArrayRemove(AEGeometry *g, Array<AEGeometry*> *arr);  // ArrayRemove<AEGeometry*>(AEGeometry*, Array*)

void LODManager::removeObject(AEGeometry *g)
{
    for (uint32_t i = 0; i < this->objects->size(); i++) {
        Array<AEGeometry*> *arr = this->objects;
        if ((*arr)[i] == g)
            LODManager_ArrayRemove(g, arr);
    }
}

// ---- forceUpdate_952c4.cpp ----
// LODManager::forceUpdate(int, bool) — recompute the camera position and update the
// LOD level of every registered geometry. Uses the engine camera/matrix helpers and
// a stack-resident Vector (address-taken -> stack canary).

struct PaintCanvas;

__attribute__((visibility("hidden"))) extern void **g_LOD_canvas;   // *holder = PaintCanvas*
__attribute__((visibility("hidden"))) extern void *g_LOD_settings;  // float at +0x28 = LOD factor

void MatrixGetPosition(void *out, const Matrix *m);     // RetStr

void LODManager::forceUpdate(int dt, bool useParent)
{
    Vector local;
    void *canvas = *g_LOD_canvas;
    float factor = *(float*)((char*)g_LOD_settings + 0x28);

    uint32_t cam = CameraGetCurrent(canvas);
    Matrix *m = CameraGetLocal(canvas, cam);
    MatrixGetPosition(&local, m);
    this->cameraPos = local;

    for (uint32_t i = 0; i < this->objects->size(); i++) {
        AEGeometry *g = (*this->objects)[i];
        if (useParent)
            ((AEGeometry *)(&local))->getParentPosition();
        else
            local = this->cameraPos;
        ((AEGeometry *)(g))->updateLod(local, factor);
    }
}

// ---- update_95384.cpp ----
void LODManager::update(int dt)
{
    int sum = this->timer + dt;
    this->timer = sum;
    if (sum > 1000) {
        this->timer = 0;
        return forceUpdate(0, false);
    }
}
