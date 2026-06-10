#include "MTitle.h"


extern "C" void PaintCanvas_ReleaseAllResources(void *canvas);
extern "C" int GameText_getLanguage();
extern "C" void Globals_loadFont(void *obj, int lang);
extern "C" void Layout_reload(void *p);
extern "C" void ImageFactory_reload(void *p);
extern "C" void MTitle_or_tail(void *p);
extern "C" void PaintCanvas_Begin2d(void *canvas);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y, int ax, int ay);
extern "C" void PaintCanvas_End2d(void *canvas);
extern "C" void Layout_drawBG(void *layout);
extern "C" void Layout_drawHeader(void *layout);
extern "C" void Layout_drawEmptyFooter(void *layout, int flag);
extern "C" int ApplicationManager_GetElapsedTimeMillis(void *arg);
extern "C" void MTitle_r2dDone(void *screen, int arg);
extern "C" void MTitle_r2dTail(void *canvas);
extern "C" void _ZN6MTitle9OnReleaseEv(MTitle *self);
extern "C" MTitle *_ZN6MTitleD2Ev(MTitle *self);
extern "C" void MTitle_deleteTail(MTitle *self);
extern "C" void PaintCanvas_ClearBuffer(void *canvas, int value);
extern "C" void PaintCanvas_Begin3d(void *arg);
extern "C" void MTitle_r3dTail(void *arg);
extern "C" void PaintCanvas_Image2DCreate(void *canvas, unsigned short image, unsigned int *out);
extern "C" void FModSound_play(void *self, int sound, int pos, int vel, int extra);

// ---- OnRelease_97a2c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_font;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_reload;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_imgfac;


void MTitle::OnRelease()
{
    PaintCanvas_ReleaseAllResources(*g_MTitle_or_canvas);

    void *fontObj = *g_MTitle_or_font;
    Globals_loadFont(fontObj, GameText_getLanguage());

    void **reload = g_MTitle_or_reload;
    if (*reload != 0) {
        Layout_reload(*reload);
        ImageFactory_reload(*g_MTitle_or_imgfac);
        MTitle_or_tail(*reload);
    }
}

// ---- OnRender2D_97a8c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_layout;
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_screen;


void MTitle::OnRender2D()
{
    PaintCanvas_Begin2d(P(this, 0x4));

    void **canvas = g_MTitle_r2d_canvas;
    PaintCanvas_SetColor(*canvas, -1);

    void **layout = g_MTitle_r2d_layout;
    Layout_drawBG(*layout);
    Layout_drawHeader(*layout);
    Layout_drawEmptyFooter(*layout, 0);

    int t = ApplicationManager_GetElapsedTimeMillis(P(this, 0x8));
    if (0x32 < t)
        t = 0x32;
    else
        t = ApplicationManager_GetElapsedTimeMillis(P(this, 0x8));

    t += I(this, 0x1c);
    I(this, 0x1c) = t;

    int image;
    float fade;
    if (t > 0xfa0) {
        int step = *(volatile int *)B(this, 0x18) + 1;
        I(this, 0x18) = step;
        I(this, 0x1c) = 0;
        if (step == 2) {
            MTitle_r2dDone(*g_MTitle_r2d_screen, 1);
            return;
        }
        image = I(this, step == 0 ? 0x14 : 0x10);
        t = 0;
        goto pathC;
    }

    image = I(this, I(this, 0x18) == 0 ? 0x14 : 0x10);
    if (t >= 0x3e8) {
        if (t >= 0xbb9) {
            fade = (float)(t - 0xbb8) / -1000.0f + 1.0f;
            goto common;
        }
        fade = 1.0f;
        goto common;
    }
pathC:
    fade = (float)t / 1000.0f;
common:
    int color = (int)(fade * 255.0f) - 0x100;
    PaintCanvas_SetColor(P(this, 0x4), color);
    PaintCanvas_DrawImage2D(*canvas, image, 0, 0, 0x44, 0x44);
    PaintCanvas_End2d(P(this, 0x4));
    MTitle_r2dTail(P(this, 0x4));
}

// ---- MTitle_97984.cpp ----
__attribute__((visibility("hidden"))) extern void *volatile g_MTitle_vtable;

MTitle::MTitle()
{
    I(this, 0xc) = 100;
    void *vtable = g_MTitle_vtable;
    P(this, 0) = (char *)vtable + 8;
}

// ---- _MTitle_97998.cpp ----
__attribute__((visibility("hidden"))) extern void *g_MTitle_vtable;


extern "C" MTitle *_ZN6MTitleD2Ev(MTitle *self)
{
    P(self, 0) = (char *)g_MTitle_vtable + 8;
    _ZN6MTitle9OnReleaseEv(self);
    return self;
}

// ---- OnTouchEnd_97a24.cpp ----
void MTitle::OnTouchEnd(int x, int y)
{
    I(this, 0x1c) = 5000;
}

// ---- _MTitle_979bc.cpp ----
extern "C" void _ZN6MTitleD0Ev(MTitle *self)
{
    MTitle_deleteTail(_ZN6MTitleD2Ev(self));
}

// ---- OnRender3D_97bc4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_r3d_canvas;


void MTitle::OnRender3D()
{
    void **canvas = g_MTitle_r3d_canvas;
    PaintCanvas_ClearBuffer(*canvas, 0xff);
    PaintCanvas_Begin3d(P(this, 0x4));
    MTitle_r3dTail(P(this, 0x4));
}

// ---- OnInitialize_979cc.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_sound;


int MTitle::OnInitialize()
{
    void **canvas = g_MTitle_canvas;
    PaintCanvas_Image2DCreate(*canvas, 7000, (unsigned int *)B(this, 0x10));
    PaintCanvas_Image2DCreate(*canvas, 0x1b59, (unsigned int *)B(this, 0x14));

    int z = 0;
    FModSound_play(*g_MTitle_sound, 0x91, z, z, z);

    I(this, 0x18) = z;
    I(this, 0x1c) = z;
    I(this, 0xc) = 100;
    return z;
}
