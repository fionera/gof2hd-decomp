#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodChildMeshes(unsigned short*)
extern "C" void _ae_TransformAddMesh(uint32_t canvas, uint32_t tf, uint16_t mesh, int z);
extern "C" void _ae_TransformRemoveChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);

struct AEGeometry { void setLodChildMeshes(uint16_t *meshes); };
void AEGeometry::setLodChildMeshes(uint16_t *meshes)
{
    int count = this->f_50;
    if (count > 0) {
        uint16_t *m = new uint16_t[count];
        this->f_60 = m;
        uint32_t *tfs = new uint32_t[count];
        this->f_58 = tfs;
        for (int i = 0; i < count; i++) {
            ((uint16_t *)this->f_60)[i] = meshes[i];
            PaintCanvas::TransformCreate((PaintCanvas *)this->f_2c,
                                         &((uint32_t *)this->f_58)[i]);
            _ae_TransformAddMesh(this->f_2c, ((uint32_t *)this->f_58)[i],
                                 (uint16_t)meshes[i], 0);
            PaintCanvas::TransformAddChild((PaintCanvas *)this->f_2c,
                                           ((uint32_t *)this->f_54)[i],
                                           ((uint32_t *)this->f_58)[i]);
            _ae_TransformRemoveChild((PaintCanvas *)this->f_2c,
                                     ((uint32_t *)this->f_54)[i], this->f_14);
            PaintCanvas::TransformAddChild((PaintCanvas *)this->f_2c,
                                           ((uint32_t *)this->f_54)[i], this->f_14);
            count = this->f_50;
        }
    }
}
