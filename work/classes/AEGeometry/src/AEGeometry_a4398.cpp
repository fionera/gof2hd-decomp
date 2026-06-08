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
    this->f_78 = 0;
    this->f_7c = 0;
    this->f_80 = 0;
    new ((Matrix *)((char *)this + 0x84)) Matrix();
    this->f_2c = canvas;
    *(uint16_t *)((char *)this + 8) = mesh;
    this->f_c = 0;
    this->f_18 = 0;
    PaintCanvas::TransformCreate(canvas, &this->f_18);
    _ae_MeshCreate(canvas, mesh, &this->f_1c, flag);
    _ae_TransformAddMeshId(canvas, this->f_18, this->f_1c);

    *(V4b *)((char *)this + 0x30) = kDir2;
    *(V4b *)((char *)this + 0x54) = (V4b){0, 0, 0, 0};
    this->f_40 = 1.0f;
    this->f_44 = 1.0f;
    *(uint16_t *)((char *)this + 0x48) = 0x101;
    this->f_4c = 0;
    this->f_64 = 0;
    this->f_20 = 0xffffffff;
    this->f_24 = 0xffffffff;
    this->f_28 = 0;
    this->f_c = this->f_18;
    this->f_10 = 0xffffffff;
    this->f_14 = 0xffffffff;
    uint32_t loc = PaintCanvas::TransformGetLocal((uint32_t)canvas, 0);
    *(Matrix *)((char *)this + 0x84) = *(Matrix *)loc;
    this->f_4 = 0;
}
