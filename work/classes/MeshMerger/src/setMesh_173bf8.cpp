#include "class.h"

extern "C" void PaintCanvas_MeshCreate(void *canvas, uint16_t meshId, uint32_t *out, bool flag);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t id);

void MeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    void *canvas = this->f_c;
    PaintCanvas_MeshCreate(canvas, meshId, &id, false);
    void *ptr = PaintCanvas_MeshGetPointer(this->f_c, id);
    void **meshes = (void **)this->f_8;
    meshes[this->f_0 * lod + index] = ptr;
}
