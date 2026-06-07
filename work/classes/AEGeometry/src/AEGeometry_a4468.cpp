#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::AEGeometry(PaintCanvas*)
struct AEGeometry { AEGeometry(PaintCanvas *canvas); };

struct V4 { float a, b, c, d; };
__attribute__((visibility("hidden"))) static const V4 kDir = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(PaintCanvas *canvas)
{
    u32(this, 0x78) = 0;
    u32(this, 0x7c) = 0;
    u32(this, 0x80) = 0;
    new ((Matrix *)((char *)this + 0x84)) Matrix();
    *(uint16_t *)((char *)this + 8) = 0;
    pp(this, 0x2c) = canvas;
    PaintCanvas::TransformCreate(canvas, &u32(this, 0x18));

    *(V4 *)((char *)this + 0x30) = kDir;
    *(V4 *)((char *)this + 0x54) = (V4){0.0f, 0.0f, 0.0f, 0.0f};

    f32(this, 0x40) = 1.0f;
    f32(this, 0x44) = 1.0f;
    *(uint16_t *)((char *)this + 0x48) = 0x101;
    u32(this, 0x4c) = 0;
    u32(this, 0x64) = 0;
    u32(this, 0x1c) = 0xffffffff;
    u32(this, 0x20) = 0xffffffff;
    u32(this, 0x24) = 0xffffffff;
    u32(this, 0x28) = 0;
    u32(this, 4) = 0;
    u32(this, 0xc) = u32(this, 0x18);
    u32(this, 0x10) = 0xffffffff;
    u32(this, 0x14) = 0xffffffff;
}
