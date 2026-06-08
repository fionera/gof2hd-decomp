#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::AEGeometry(PaintCanvas*)
struct AEGeometry { AEGeometry(PaintCanvas *canvas); };

struct V4 { float a, b, c, d; };
__attribute__((visibility("hidden"))) static const V4 kDir = {0.0f, 0.0f, 0.0f, 1.0f};

AEGeometry::AEGeometry(PaintCanvas *canvas)
{
    this->f_78 = 0;
    this->f_7c = 0;
    this->f_80 = 0;
    new ((Matrix *)((char *)this + 0x84)) Matrix();
    *(uint16_t *)((char *)this + 8) = 0;
    this->f_2c = canvas;
    PaintCanvas::TransformCreate(canvas, &this->f_18);

    *(V4 *)((char *)this + 0x30) = kDir;
    *(V4 *)((char *)this + 0x54) = (V4){0.0f, 0.0f, 0.0f, 0.0f};

    this->f_40 = 1.0f;
    this->f_44 = 1.0f;
    *(uint16_t *)((char *)this + 0x48) = 0x101;
    this->f_4c = 0;
    this->f_64 = 0;
    this->f_1c = 0xffffffff;
    this->f_20 = 0xffffffff;
    this->f_24 = 0xffffffff;
    this->f_28 = 0;
    this->f_4 = 0;
    this->f_c = this->f_18;
    this->f_10 = 0xffffffff;
    this->f_14 = 0xffffffff;
}
