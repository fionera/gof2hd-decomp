#include "class.h"

extern "C" void *AEGeometry_dtor(void *geom);              // 0x71fc8
extern "C" void *Level_dtor(void *level);                  // 0x71fd4
extern "C" void operator_delete(void *p);                  // 0x6eb48
extern "C" void PaintCanvas_FogEnable(void *canvas, int a, int b); // 0x71fe0
extern "C" void ArrayReleaseClasses_AEGeometryPtr(void *arr);      // 0x71fec
extern "C" void *Array_AEGeometryPtr_dtor(void *arr);             // 0x717c4

__attribute__((visibility("hidden"))) extern void **g_canvasFog; // FogEnable target

CutScene::~CutScene()
{
    void *g = pp(this, 0x20);
    if (g != 0) operator_delete(AEGeometry_dtor(g));
    pp(this, 0x20) = 0;

    g = pp(this, 0x64);
    if (g != 0) operator_delete(AEGeometry_dtor(g));
    pp(this, 0x64) = 0;

    g = pp(this, 0x0);
    if (g != 0) operator_delete(Level_dtor(g));
    pp(this, 0x0) = 0;

    PaintCanvas_FogEnable(*g_canvasFog, 0, 1);

    g = pp(this, 0x28);
    if (g != 0) operator_delete(AEGeometry_dtor(g));
    pp(this, 0x28) = 0;

    g = pp(this, 0x2c);
    if (g != 0) operator_delete(AEGeometry_dtor(g));
    pp(this, 0x2c) = 0;

    g = pp(this, 0x30);
    if (g != 0) operator_delete(AEGeometry_dtor(g));
    pp(this, 0x30) = 0;

    g = pp(this, 0x34);
    if (g != 0) operator_delete(AEGeometry_dtor(g));
    pp(this, 0x34) = 0;

    void *arr = pp(this, 0x38);
    if (arr != 0) {
        ArrayReleaseClasses_AEGeometryPtr(arr);
        arr = pp(this, 0x38);
        if (arr != 0) operator_delete(Array_AEGeometryPtr_dtor(arr));
    }
    pp(this, 0x38) = 0;
}
