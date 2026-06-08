#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodMeshesWithMeshIds(unsigned short* meshes, unsigned int* meshIds, int* dists, int count)
extern "C" void _ae_TransformAddMeshId(uint32_t canvas, uint32_t tf, uint32_t meshId);

struct AEGeometry {
    void setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count);
};
void AEGeometry::setLodMeshesWithMeshIds(uint16_t *meshes, uint32_t *meshIds, int *dists, int count)
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
        _ae_TransformAddMeshId(this->f_2c, ((uint32_t *)this->f_54)[i], meshIds[i]);
        long long v = ((long long *)this->f_64)[i];
        ((long long *)this->f_64)[i] = v * v;
        if (this->f_14 != 0xffffffffu)
            PaintCanvas::TransformAddChild((PaintCanvas *)this->f_2c,
                                           ((uint32_t *)this->f_54)[i], this->f_14);
    }
}
