#include "class.h"

struct TouchSlider {
    TouchSlider(int param_1, int param_2, int param_3, float param_4);
    void setPosition(int param_1, int param_2);
};

extern "C" void PaintCanvas_Image2DCreate(void *self, unsigned short image, unsigned int *out);
extern "C" int PaintCanvas_GetImage2DWidth(void *self, int image);
extern "C" int PaintCanvas_GetImage2DHeight(void *self, int image);

// PaintCanvas singleton holder (pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_canvas;
// App/engine singleton: pc-rel -> &g; *g -> object pointer; object[0x7c].
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_app;

TouchSlider::TouchSlider(int param_1, int param_2, int param_3, float param_4)
{
    F<int>(this, 0x10) = param_1;
    F<float>(this, 0x24) = param_4;
    void **holder = g_TouchSlider_canvas;
    PaintCanvas_Image2DCreate(*holder, 0x51a, &F<unsigned int>(this, 0x30));

    unsigned short uVar4 = 0x51b;
    if (param_1 == 1)
        uVar4 = 0x51c;
    if (param_1 == 0)
        uVar4 = 0x519;
    PaintCanvas_Image2DCreate(*holder, uVar4, &F<unsigned int>(this, 0x2c));

    F<int>(this, 0x14) = PaintCanvas_GetImage2DWidth(*holder, F<int>(this, 0x30));
    F<int>(this, 0x18) = PaintCanvas_GetImage2DHeight(*holder, F<int>(this, 0x30));
    F<int>(this, 0x1c) = PaintCanvas_GetImage2DWidth(*holder, F<int>(this, 0x2c));
    F<int>(this, 0x20) = PaintCanvas_GetImage2DHeight(*holder, F<int>(this, 0x2c));

    F<unsigned short>(this, 0x34) = 0;
    setPosition(param_2, param_3);
    F<int>(this, 0x28) = 0;
    F<int>(this, 0x38) = *(int *)((char *)*g_TouchSlider_app + 0x7c);
}
