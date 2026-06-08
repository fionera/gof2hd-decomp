#include "class.h"

extern "C" void PaintCanvas_MeshCreate(void *canvas, uint16_t meshId, uint32_t *out, bool flag);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t id);

void MeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    void *canvas = pp(this, 0xc);
    PaintCanvas_MeshCreate(canvas, meshId, &id, false);
    void *ptr = PaintCanvas_MeshGetPointer(pp(this, 0xc), id);
    void **meshes = (void **)pp(this, 0x8);
    meshes[i32(this, 0x0) * lod + index] = ptr;
}
