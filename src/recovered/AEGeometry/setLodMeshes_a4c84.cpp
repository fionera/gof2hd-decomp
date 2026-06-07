#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodMeshes(unsigned short* meshes, int* dists, int count)
extern "C" void _ae_TransformAddMesh(uint32_t canvas, uint32_t tf, uint16_t mesh, int z);

struct AEGeometry { void setLodMeshes(uint16_t *meshes, int *dists, int count); };
void AEGeometry::setLodMeshes(uint16_t *meshes, int *dists, int count)
{
    uint16_t *meshArr = new uint16_t[count];
    pp(this, 0x5c) = meshArr;
    long long *distArr = new long long[count];
    pp(this, 0x64) = distArr;
    i32(this, 0x50) = count;
    uint32_t *tfArr = new uint32_t[count];
    pp(this, 0x54) = tfArr;
    for (int i = 0; i < count; i++) {
        ((uint16_t *)u32(this, 0x5c))[i] = meshes[i];
        long long *d = (long long *)u32(this, 0x64);
        d[i] = (long long)dists[i];
        PaintCanvas::TransformCreate((PaintCanvas *)u32(this, 0x2c),
                                     &((uint32_t *)u32(this, 0x54))[i]);
        _ae_TransformAddMesh(u32(this, 0x2c), ((uint32_t *)u32(this, 0x54))[i],
                             (uint16_t)meshes[i], 0);
        long long v = ((long long *)u32(this, 0x64))[i];
        ((long long *)u32(this, 0x64))[i] = v * v;
        if (u32(this, 0x14) != 0xffffffffu)
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
    }
}
