#include "gof2/TouchSlider.h"


extern "C" void PaintCanvas_Image2DCreate(void *self, unsigned short image, int *out);
extern "C" int PaintCanvas_GetImage2DWidth(void *self, int image);
extern "C" int PaintCanvas_GetImage2DHeight(void *self, int image);
extern "C" void PaintCanvas_SetColor(void *canvas, int color);
extern "C" void PaintCanvas_DrawImage2D(void *canvas, int image, int x, int y);
extern "C" void PaintCanvas_DrawImage2D6(void *canvas, int image, int x, int y, int a, int b);

// ---- setPosition_a232c.cpp ----
void TouchSlider::setPosition(int param_1, int param_2)
{
    float fVar1 = (float)(this->field_0x1c - this->field_0x14);
    float fVar2 = (float)(param_1 + this->field_0x14 / 2);
    this->field_0x0 = param_1;
    this->field_0x4 = param_2;
    this->field_0xc = param_2 + this->field_0x20 / 2;
    this->field_0x8 = (int)(fVar2 + this->field_0x24 * fVar1);
}

// ---- TouchSlider_a228c.cpp ----
// PaintCanvas singleton holder (pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_canvas;
// App/engine singleton: pc-rel -> &g; *g -> object pointer; object[0x7c].
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_app;

TouchSlider::TouchSlider(int param_1, int param_2, int param_3, float param_4)
{
    this->field_0x10 = param_1;
    this->field_0x24 = param_4;
    void **holder = g_TouchSlider_canvas;
    PaintCanvas_Image2DCreate(*holder, 0x51a, &this->field_0x30);

    unsigned short uVar4 = 0x51b;
    if (param_1 == 1)
        uVar4 = 0x51c;
    if (param_1 == 0)
        uVar4 = 0x519;
    PaintCanvas_Image2DCreate(*holder, uVar4, &this->field_0x2c);

    this->field_0x14 = PaintCanvas_GetImage2DWidth(*holder, this->field_0x30);
    this->field_0x18 = PaintCanvas_GetImage2DHeight(*holder, this->field_0x30);
    this->field_0x1c = PaintCanvas_GetImage2DWidth(*holder, this->field_0x2c);
    this->field_0x20 = PaintCanvas_GetImage2DHeight(*holder, this->field_0x2c);

    this->field_0x34 = 0;
    setPosition(param_2, param_3);
    this->field_0x28 = 0;
    this->field_0x38 = *(int *)((char *)*g_TouchSlider_app + 0x7c);
}

// ---- OnTouchBegin_a242c.cpp ----
int TouchSlider::OnTouchBegin(int param_1, int param_2)
{
    if (this->field_0x35 != 0)
        return 0;
    int r = touchedInside(param_1, param_2);
    this->field_0x34 = (uint8_t)r;
    return r;
}

// ---- OnTouchEnd_a24dc.cpp ----
int TouchSlider::OnTouchEnd(int param_1, int param_2)
{
    if (this->field_0x35 != 0)
        return 0;

    int uVar1;
    if (this->field_0x34 == 0) {
        uVar1 = 0;
    } else {
        int iVar4 = this->field_0x28;
        if (iVar4 > 0) {
            float fVar6 = (float)(iVar4 + 1);
            float fVar2 = getValue();
            float fVar5;
            float fVar7 = 0.0f;
            int iVar3 = 0;
            while (true) {
                fVar5 = fVar2;
                if (iVar3 >= iVar4 + 2)
                    break;
                fVar5 = fVar7;
                if (fVar2 <= (1.0f / fVar6) * 0.5f + fVar7)
                    break;
                fVar7 = 1.0f / fVar6 + fVar7;
                iVar3 = iVar3 + 1;
            }
            setValue(fVar5);
        }
        uVar1 = 1;
    }
    this->field_0x34 = 0;
    return uVar1;
}

// ---- draw_a2378.cpp ----
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_canvas;

void TouchSlider::draw()
{
    void **holder = g_TouchSlider_canvas;
    int color = this->field_0x35 != 0 ? 0xFFFFFF2F : -1;
    PaintCanvas_SetColor(*holder, color);
    PaintCanvas_DrawImage2D(*holder, this->field_0x2c, this->field_0x0, this->field_0x4);
    PaintCanvas_DrawImage2D6(*holder, this->field_0x30, this->field_0x8, this->field_0xc, 0x11, 0x44);
}

// ---- OnTouchMove_a2494.cpp ----
bool TouchSlider::OnTouchMove(int param_1, int param_2)
{
    if (this->field_0x35 != 0)
        return false;
    if (this->field_0x34 != 0) {
        int half = this->field_0x14 / 2;
        int lo = this->field_0x0 + half;
        int hi = (this->field_0x0 + this->field_0x1c) - half;
        if (param_1 < hi)
            hi = param_1;
        if (hi <= lo)
            hi = lo;
        this->field_0x8 = hi;
    }
    return this->field_0x34 != 0;
}

// ---- getValue_a23c0.cpp ----
float TouchSlider::getValue()
{
    float fVar1 = (float)(this->field_0x1c - this->field_0x14);
    float fVar2 = (float)((this->field_0x8 - this->field_0x14 / 2) - this->field_0x0);
    return fVar2 / fVar1;
}

// ---- setHalfTransparent_a2426.cpp ----
void TouchSlider::setHalfTransparent(bool param_1)
{
    this->field_0x35 = param_1;
}

// ---- touchedInside_a2448.cpp ----
int TouchSlider::touchedInside(int param_1, int param_2)
{
    int half = this->field_0x14 >> 1;
    int cx = this->field_0x8;
    int pad = this->field_0x38;
    if (cx - half - pad > param_1)
        return 0;
    if (param_1 >= half + cx + pad)
        return 0;
    int half2 = this->field_0x18 >> 1;
    int cy = this->field_0xc;
    if (cy - pad - half2 > param_2)
        return 0;
    return param_2 <= half2 + cy + pad;
}

// ---- setValue_a23ee.cpp ----
void TouchSlider::setValue(float param_1)
{
    float fVar1 = (float)(this->field_0x1c - this->field_0x14);
    float fVar2 = (float)(this->field_0x0 + this->field_0x14 / 2);
    this->field_0x8 = (int)(fVar2 + fVar1 * param_1);
}
