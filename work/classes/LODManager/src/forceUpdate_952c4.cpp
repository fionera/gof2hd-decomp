#include "class.h"

// LODManager::forceUpdate(int, bool) — recompute the camera position and update the
// LOD level of every registered geometry. Uses the engine camera/matrix helpers and
// a stack-resident Vector (address-taken -> stack canary).

struct PaintCanvas;

__attribute__((visibility("hidden"))) extern void **g_LOD_canvas;   // *holder = PaintCanvas*
__attribute__((visibility("hidden"))) extern void *g_LOD_settings;  // float at +0x28 = LOD factor

extern "C" uint32_t CameraGetCurrent(void *canvas);
extern "C" Matrix *CameraGetLocal(void *canvas, uint32_t index);
extern "C" void MatrixGetPosition(void *out, const Matrix *m);     // RetStr
extern "C" void Vector_assign(Vector *dst, const void *src);       // Vector::operator=
extern "C" void AEGeometry_getParentPosition(void *out, AEGeometry *g);  // RetStr
extern "C" void AEGeometry_updateLod(AEGeometry *g, const void *pos, float factor);

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
