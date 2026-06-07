#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::AEGeometry(unsigned short mesh, PaintCanvas* canvas, bool flag)
extern "C" void _ae_MeshCreate(PaintCanvas *c, uint16_t mesh, uint32_t *out, bool flag);
extern "C" void _ae_TransformAddMeshId(PaintCanvas *c, uint32_t tf, uint32_t meshId);

struct AEGeometry { AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag); };

struct V4b { float a, b, c, d; };
__attribute__((visibility("hidden"))) static const V4b kDir2 = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(uint16_t mesh, PaintCanvas *canvas, bool flag)
{
    u32(this, 0x78) = 0;
    u32(this, 0x7c) = 0;
    u32(this, 0x80) = 0;
    new ((Matrix *)((char *)this + 0x84)) Matrix();
    pp(this, 0x2c) = canvas;
    *(uint16_t *)((char *)this + 8) = mesh;
    u32(this, 0xc) = 0;
    u32(this, 0x18) = 0;
    PaintCanvas::TransformCreate(canvas, &u32(this, 0x18));
    _ae_MeshCreate(canvas, mesh, &u32(this, 0x1c), flag);
    _ae_TransformAddMeshId(canvas, u32(this, 0x18), u32(this, 0x1c));

    *(V4b *)((char *)this + 0x30) = kDir2;
    *(V4b *)((char *)this + 0x54) = (V4b){0, 0, 0, 0};
    f32(this, 0x40) = 1.0f;
    f32(this, 0x44) = 1.0f;
    *(uint16_t *)((char *)this + 0x48) = 0x101;
    u32(this, 0x4c) = 0;
    u32(this, 0x64) = 0;
    u32(this, 0x20) = 0xffffffff;
    u32(this, 0x24) = 0xffffffff;
    u32(this, 0x28) = 0;
    u32(this, 0xc) = u32(this, 0x18);
    u32(this, 0x10) = 0xffffffff;
    u32(this, 0x14) = 0xffffffff;
    uint32_t loc = PaintCanvas::TransformGetLocal((uint32_t)canvas, 0);
    *(Matrix *)((char *)this + 0x84) = *(Matrix *)loc;
    u32(this, 4) = 0;
}
