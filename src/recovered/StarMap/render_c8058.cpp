#include "class.h"

extern "C" __attribute__((visibility("hidden"))) uint32_t *g_StarMap_render_canvas;
extern "C" __attribute__((visibility("hidden"))) void (*g_StarMap_render_geometry)(void *);

extern "C" void PaintCanvas_SetColor(uint32_t canvas, uint32_t color);
extern "C" void PaintCanvas_End3d(uint32_t canvas);
extern "C" void PaintCanvas_Begin3d(uint32_t canvas);
extern "C" void PaintCanvas_SetTexture(uint32_t canvas, uint32_t texture, uint32_t color);
extern "C" void PaintCanvas_SetBlendMode(uint32_t canvas, int mode);
extern "C" void AEGeometry_render(void *geometry);
extern "C" void StarMap_render_tail();

void StarMap::render()
{
    uint32_t *canvasHolder = g_StarMap_render_canvas;
    PaintCanvas_SetColor(*canvasHolder, 0xffffffffu);
    void (*renderGeometry)(void *) = g_StarMap_render_geometry;
    renderGeometry(ptr_field(this, 0x1b0));
    renderGeometry(ptr_field(this, 0x1b4));
    renderGeometry(ptr_field(this, 0x1b8));
    renderGeometry(ptr_field(this, 0x6c));
    PaintCanvas_End3d(*canvasHolder);
    PaintCanvas_Begin3d(*canvasHolder);
    if (ptr_field(this, 0xa4) != 0) {
        PaintCanvas_SetTexture(*canvasHolder, field<uint32_t>(this, 0x178), 0xffffffffu);
        PaintCanvas_SetBlendMode(*canvasHolder, 2);
        AEGeometry_render(ptr_field(this, 0x1bc));
        AEGeometry_render(ptr_field(this, 0xa4));
    }
    if (ptr_field(this, 0xf8) != 0) {
        return StarMap_render_tail();
    }
}
