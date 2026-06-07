#include "class.h"

using namespace AbyssEngine;

// AEGeometry::setLodChildMeshes(unsigned short*)
extern "C" void _ae_TransformAddMesh(uint32_t canvas, uint32_t tf, uint16_t mesh, int z);
extern "C" void _ae_TransformRemoveChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);

struct AEGeometry { void setLodChildMeshes(uint16_t *meshes); };
void AEGeometry::setLodChildMeshes(uint16_t *meshes)
{
    int count = i32(this, 0x50);
    if (count > 0) {
        uint16_t *m = new uint16_t[count];
        pp(this, 0x60) = m;
        uint32_t *tfs = new uint32_t[count];
        pp(this, 0x58) = tfs;
        for (int i = 0; i < count; i++) {
            ((uint16_t *)u32(this, 0x60))[i] = meshes[i];
            PaintCanvas::TransformCreate((PaintCanvas *)u32(this, 0x2c),
                                         &((uint32_t *)u32(this, 0x58))[i]);
            _ae_TransformAddMesh(u32(this, 0x2c), ((uint32_t *)u32(this, 0x58))[i],
                                 (uint16_t)meshes[i], 0);
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i],
                                           ((uint32_t *)u32(this, 0x58))[i]);
            _ae_TransformRemoveChild((PaintCanvas *)u32(this, 0x2c),
                                     ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
            PaintCanvas::TransformAddChild((PaintCanvas *)u32(this, 0x2c),
                                           ((uint32_t *)u32(this, 0x54))[i], u32(this, 0x14));
            count = i32(this, 0x50);
        }
    }
}
