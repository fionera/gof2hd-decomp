#include "class.h"

extern "C" void PaintCanvas_MeshCreate(void *canvas, uint16_t meshId, uint32_t *out, bool flag);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t id);

void LodMeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    PaintCanvas_MeshCreate(this->f_14, meshId, &id, false);
    void *ptr = PaintCanvas_MeshGetPointer(this->f_14, id);
    void **meshes = (void **)this->f_c;
    meshes[this->f_0 * lod + index] = ptr;
}
