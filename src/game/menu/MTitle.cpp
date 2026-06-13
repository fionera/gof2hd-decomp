#include "gof2/game/menu/MTitle.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/engine/core/ApplicationManager.h"
#include "gof2/engine/render/ImageFactory.h"
#include "gof2/game/ui/Layout.h"
#include "gof2/engine/render/PaintCanvas.h"


int GameText_getLanguage();
void Globals_loadFont(void *obj, int lang);
extern "C" void _ZN6MTitle9OnReleaseEv(MTitle *self);
MTitle *_ZN6MTitleD2Ev(MTitle *self);

// ---- OnRelease_97a2c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_font;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_reload;
__attribute__((visibility("hidden"))) extern void **g_MTitle_or_imgfac;


void MTitle::OnRelease()
{
    ((PaintCanvas *)*g_MTitle_or_canvas)->ReleaseAllResources();

    void *fontObj = *g_MTitle_or_font;
    Globals_loadFont(fontObj, GameText_getLanguage());

    void **reload = g_MTitle_or_reload;
    if (*reload != 0) {
        ((Layout *)(*reload))->reload();
        ((ImageFactory *)(*g_MTitle_or_imgfac))->reload();
        ((MTitle *)(*reload))->or_tail();
    }
}

// ---- OnRender2D_97a8c.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_layout;
__attribute__((visibility("hidden"))) extern void **g_MTitle_r2d_screen;


void MTitle::OnRender2D()
{
    ((PaintCanvas *)this->canvas)->Begin2d();

    void **canvas = g_MTitle_r2d_canvas;
    ((PaintCanvas *)*canvas)->SetColor((unsigned int)-1);

    void **layout = g_MTitle_r2d_layout;
    ((Layout *)(*layout))->drawBG();
    ((Layout *)(*layout))->drawHeader0();
    ((Layout *)(*layout))->drawEmptyFooter(0);

    int t = ((ApplicationManager *)(this->appManager))->GetElapsedTimeMillis();
    if (0x32 < t)
        t = 0x32;
    else
        t = ((ApplicationManager *)(this->appManager))->GetElapsedTimeMillis();

    t += this->timer;
    this->timer = t;

    int image;
    float fade;
    if (t > 0xfa0) {
        int step = *(volatile int *)(&this->step) + 1;
        this->step = step;
        this->timer = 0;
        if (step == 2) {
            ((MTitle *)(*g_MTitle_r2d_screen))->r2dDone(1);
            return;
        }
        image = (int)(step == 0 ? this->logoImage2 : this->logoImage);
        t = 0;
        goto pathC;
    }

    image = (int)(this->step == 0 ? this->logoImage2 : this->logoImage);
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
    ((PaintCanvas *)this->canvas)->SetColor((unsigned int)color);
    ((PaintCanvas *)*canvas)->DrawImage2D(image, 0, 0, (unsigned char)0x44, (unsigned char)0x44);
    ((PaintCanvas *)this->canvas)->End2d();
    ((MTitle *)(this->canvas))->r2dTail();
}

// ---- MTitle_97984.cpp ----
__attribute__((visibility("hidden"))) extern void *volatile g_MTitle_vtable;

MTitle::MTitle()
{
    this->renderPriority = 100;
    void *vtable = g_MTitle_vtable;
    this->field_0x0 = (char *)vtable + 8;
}

// ---- _MTitle_97998.cpp ----
MTitle *_ZN6MTitleD2Ev(MTitle *self)
{
    self->field_0x0 = (char *)g_MTitle_vtable + 8;
    _ZN6MTitle9OnReleaseEv(self);
    return self;
}

// ---- OnTouchEnd_97a24.cpp ----
void MTitle::OnTouchEnd(int x, int y)
{
    this->timer = 5000;
}

// ---- _MTitle_979bc.cpp ----
void _ZN6MTitleD0Ev(MTitle *self)
{
    ((MTitle *)(_ZN6MTitleD2Ev(self)))->deleteTail();
}

// ---- OnRender3D_97bc4.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_r3d_canvas;


void MTitle::OnRender3D()
{
    void **canvas = g_MTitle_r3d_canvas;
    ((PaintCanvas *)*canvas)->ClearBuffer(0xff);
    ((PaintCanvas *)this->canvas)->Begin3d();
    ((MTitle *)(this->canvas))->r3dTail();
}

// ---- tail fragments ----
// Each of these is an ARM->Thumb interworking-return veneer in the binary that
// wraps exactly one concrete engine call (resolved via the trampoline pool at
// 0x1ab8f8 / 0x1ab908 / 0x1ab918 / 0x1ab928 / 0x1ab0a8). They are kept as
// dedicated tail methods so the recovered OnRelease/OnRender* bodies stay
// faithful to the original control flow while remaining real engine calls.

// OnRelease tail: after the active Layout and the ImageFactory have been
// reloaded, finish by reloading the screen's image factory resources. The
// receiver is the ImageFactory the veneer was invoked on.
void MTitle::or_tail()
{
    ((ImageFactory *)this)->reload();
}

// OnRender2D: the two-frame logo intro has elapsed (step == 2); hand control to
// the next application module. The receiver is the ApplicationManager.
void MTitle::r2dDone(int moduleId)
{
    ((ApplicationManager *)this)->SetCurrentApplicationModule((unsigned)moduleId);
}

// OnRender2D tail: close the 2D pass on the canvas (receiver is the PaintCanvas).
void MTitle::r2dTail()
{
    ((PaintCanvas *)this)->End2d();
}

// OnRender3D tail: open (and so flush) the 3D pass on the canvas (receiver is
// the PaintCanvas).
void MTitle::r3dTail()
{
    ((PaintCanvas *)this)->Begin3d();
}

// Deleting-destructor tail: the complete-object destructor has run, free the
// MTitle storage.
void MTitle::deleteTail()
{
    ::operator delete(this);
}

// ---- OnInitialize_979cc.cpp ----
__attribute__((visibility("hidden"))) extern void **g_MTitle_canvas;
__attribute__((visibility("hidden"))) extern void **g_MTitle_sound;


int MTitle::OnInitialize()
{
    void **canvas = g_MTitle_canvas;
    ((PaintCanvas *)*canvas)->Image2DCreate(7000, (unsigned int *)(&this->logoImage));
    ((PaintCanvas *)*canvas)->Image2DCreate(0x1b59, (unsigned int *)(&this->logoImage2));

    int z = 0;
    ((FModSound *)(*g_MTitle_sound))->play(0x91, (Vector *)z, (Vector *)z, (float)z);

    this->step = z;
    this->timer = z;
    this->renderPriority = 100;
    return z;
}
