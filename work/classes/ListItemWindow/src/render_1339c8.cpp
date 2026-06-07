#include "class.h"

extern "C" void  _liw_Begin3d();                                       // 0x71da0
extern "C" void  _liw_EnableClip(void *c, int x, int y, int w, int h); // 0x75b38
extern "C" void  _liw_SetColor(void *c, int color);                    // 0x6fac0
extern "C" void *_liw_CameraGetLocal(void *c);                         // 0x6ff1c
extern "C" void  _liw_Matrix_assign(void *dst, void *src);             // 0x6f148
extern "C" void  _liw_AEGeometry_render(void *g);                      // 0x72238
extern "C" void  _liw_End3d(void *c);                                  // 0x71dac
extern "C" void  _liw_render_tail(void *c, int a, int h, void *sp);    // 0x1ac428 tail

__attribute__((visibility("hidden"))) extern void **g_liw_r_canvas;   // 0x1439e0
__attribute__((visibility("hidden"))) extern void **g_liw_r_obj;      // 0x1439ec

// ListItemWindow::render()
struct ListItemWindow { void render(); };
void ListItemWindow::render()
{
    if (!u8(this, 0x54))
        return;

    void *canvas = *g_liw_r_canvas;
    _liw_Begin3d();

    int *obj = (int *)*g_liw_r_obj;
    int s = obj[0x128 / 4];
    int h = i32(this, 0x20) - s * 2;
    _liw_EnableClip(canvas,
        i32(this, 0x64) + s + (i32(this, 0x6c) >> 1) + obj[0x2c / 4],
        i32(this, 0x68) + s + obj[0xc / 4] + obj[0x20 / 4],
        ((i32(this, 0x6c) >> 1) - (obj[0x2c / 4] + s * 2)) - obj[0x28 / 4],
        h);
    _liw_SetColor(canvas, -1);
    void *m = _liw_CameraGetLocal(canvas);
    _liw_Matrix_assign((char *)this + 0x98, m);
    _liw_AEGeometry_render(pp(this, 0x10));
    _liw_End3d(canvas);
    int dummy;
    return _liw_render_tail(canvas, 0, h, &dummy);
}
