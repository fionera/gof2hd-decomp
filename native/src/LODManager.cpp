#include "LODManager.h"


extern "C" void LODManager_addObject_tail(AEGeometry *g, void *objects);
extern "C" void LODManager_operator_delete(void *p);
extern "C" uint32_t CameraGetCurrent(void *canvas);
extern "C" Matrix *CameraGetLocal(void *canvas, uint32_t index);
extern "C" void AEGeometry_updateLod(AEGeometry *g, const void *pos, float factor);

// ---- addObject_9520c.cpp ----
extern "C" int LODManager_hasLod(AEGeometry *g);              // AEGeometry::hasLod()

struct LODManager {
    void *objects;
    void addObject(AEGeometry *g);
};

void LODManager::addObject(AEGeometry *g)
{
    if (LODManager_hasLod(g) == 0)
        return;
    return LODManager_addObject_tail(g, this->objects);
}

// ---- LODManager_9518c.cpp ----
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

// ---- _LODManager_951dc.cpp ----
// LODManager::~LODManager() — real C++ destructor so the demangled symbol contains "~LODManager".

extern "C" void *LODManager_Array_dtor(void *p);   // Array<AEGeometry*>::~Array(Array*)

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

// ---- removeObject_9524e.cpp ----
struct LODArray {
    uint32_t length;
    AEGeometry **data;
};

extern "C" void LODManager_ArrayRemove(AEGeometry *g, LODArray *arr);  // ArrayRemove<AEGeometry*>(AEGeometry*, Array*)

struct LODManager {
    LODArray *objects;
    void removeObject(AEGeometry *g);
};

void LODManager::removeObject(AEGeometry *g)
{
    for (uint32_t i = 0; i < this->objects->length; i++) {
        LODArray *arr = this->objects;
        if (arr->data[i] == g)
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

extern "C" void MatrixGetPosition(void *out, const Matrix *m);     // RetStr
extern "C" void Vector_assign(Vector *dst, const void *src);       // Vector::operator=
extern "C" void AEGeometry_getParentPosition(void *out, AEGeometry *g);  // RetStr

struct LODArray {
    uint32_t length;
    AEGeometry **data;
};

struct LODManager {
    LODArray *objects;
    Vector cameraPos;   // +0x04
    void forceUpdate(int dt, bool useParent);
};

void LODManager::forceUpdate(int dt, bool useParent)
{
    char local[12];
    void *canvas = *g_LOD_canvas;
    float factor = F<float>(g_LOD_settings, 0x28);

    uint32_t cam = CameraGetCurrent(canvas);
    Matrix *m = CameraGetLocal(canvas, cam);
    MatrixGetPosition(local, m);
    Vector_assign(&this->cameraPos, local);

    for (uint32_t i = 0; i < this->objects->length; i++) {
        AEGeometry *g = this->objects->data[i];
        if (useParent)
            AEGeometry_getParentPosition(local, g);
        else
            *(Vector *)local = this->cameraPos;
        AEGeometry_updateLod(g, local, factor);
    }
}

// ---- update_95384.cpp ----
struct LODManager {
    void update(int dt);
    void forceUpdate(int a, bool b);
};

void LODManager::update(int dt)
{
    int sum = F<int>(this, 0x10) + dt;
    F<int>(this, 0x10) = sum;
    if (sum > 1000) {
        F<int>(this, 0x10) = 0;
        return forceUpdate(0, false);
    }
}
