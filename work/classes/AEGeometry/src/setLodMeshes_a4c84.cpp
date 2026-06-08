#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodMeshes(unsigned short* meshes, int* dists, int count)
extern "C" void _ae_TransformAddMesh(uint32_t canvas, uint32_t tf, uint16_t mesh, int z);

struct AEGeometry { void setLodMeshes(uint16_t *meshes, int *dists, int count); };
void AEGeometry::setLodMeshes(uint16_t *meshes, int *dists, int count)
{
    uint16_t *meshArr = new uint16_t[count];
    this->f_5c = meshArr;
    long long *distArr = new long long[count];
    this->f_64 = distArr;
    this->f_50 = count;
    uint32_t *tfArr = new uint32_t[count];
    this->f_54 = tfArr;
    for (int i = 0; i < count; i++) {
        ((uint16_t *)this->f_5c)[i] = meshes[i];
        long long *d = (long long *)this->f_64;
        d[i] = (long long)dists[i];
        PaintCanvas::TransformCreate((PaintCanvas *)this->f_2c,
                                     &((uint32_t *)this->f_54)[i]);
        _ae_TransformAddMesh(this->f_2c, ((uint32_t *)this->f_54)[i],
                             (uint16_t)meshes[i], 0);
        long long v = ((long long *)this->f_64)[i];
        ((long long *)this->f_64)[i] = v * v;
        if (this->f_14 != 0xffffffffu)
            PaintCanvas::TransformAddChild((PaintCanvas *)this->f_2c,
                                           ((uint32_t *)this->f_54)[i], this->f_14);
    }
}
