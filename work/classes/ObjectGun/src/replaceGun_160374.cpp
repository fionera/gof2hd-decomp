#include "class.h"

extern "C" void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
extern "C" void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
__attribute__((visibility("hidden"))) extern "C" void *g_PaintCanvas;

void ObjectGun::replaceGun(int mesh)
{
    void **canvas = (void **)g_PaintCanvas;
    TransformRemoveMesh(*canvas, this->f_10, this->f_28);
    this->f_28 = mesh;
    return TransformAddMesh(*canvas, this->f_10, (uint16_t)mesh, 0);
}
