#include "class.h"

extern "C" void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
extern "C" void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;

void ObjectGun::replaceGun(int mesh)
{
    void **canvas = (void **)g_PaintCanvas;
    TransformRemoveMesh(*canvas, F<uint32_t>(this, 0x10), F<uint16_t>(this, 0x28));
    F<int>(this, 0x28) = mesh;
    return TransformAddMesh(*canvas, F<uint32_t>(this, 0x10), (uint16_t)mesh, 0);
}
