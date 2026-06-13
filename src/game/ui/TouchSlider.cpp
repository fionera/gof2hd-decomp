#include "gof2/TouchSlider.h"
#include "gof2/PaintCanvas.h"

// ---- setPosition_a232c.cpp ----
void TouchSlider::setPosition(int param_1, int param_2)
{
    float fVar1 = (float)(this->trackWidth - this->knobWidth);
    float fVar2 = (float)(param_1 + this->knobWidth / 2);
    this->x = param_1;
    this->y = param_2;
    this->knobY = param_2 + this->trackHeight / 2;
    this->knobX = (int)(fVar2 + this->value * fVar1);
}

// ---- TouchSlider_a228c.cpp ----
// PaintCanvas singleton holder (pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_canvas;
// App/engine singleton: pc-rel -> &g; *g -> object pointer; object[0x7c].
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_app;

TouchSlider::TouchSlider(int param_1, int param_2, int param_3, float param_4)
{
    this->type = param_1;
    this->value = param_4;
    void **holder = g_TouchSlider_canvas;
    ((PaintCanvas*)*holder)->Image2DCreate(0x51a, (unsigned int*)&this->knobImage);

    unsigned short uVar4 = 0x51b;
    if (param_1 == 1)
        uVar4 = 0x51c;
    if (param_1 == 0)
        uVar4 = 0x519;
    ((PaintCanvas*)*holder)->Image2DCreate(uVar4, (unsigned int*)&this->trackImage);

    this->knobWidth = ((PaintCanvas*)*holder)->GetImage2DWidth(this->knobImage);
    this->knobHeight = ((PaintCanvas*)*holder)->GetImage2DHeight(this->knobImage);
    this->trackWidth = ((PaintCanvas*)*holder)->GetImage2DWidth(this->trackImage);
    this->trackHeight = ((PaintCanvas*)*holder)->GetImage2DHeight(this->trackImage);

    this->isDragging = 0;
    setPosition(param_2, param_3);
    this->numSteps = 0;
    this->touchPadding = *(int *)((char *)*g_TouchSlider_app + 0x7c);
}

// ---- OnTouchBegin_a242c.cpp ----
int TouchSlider::OnTouchBegin(int param_1, int param_2)
{
    if (this->isDisabled != 0)
        return 0;
    int r = touchedInside(param_1, param_2);
    this->isDragging = (uint8_t)r;
    return r;
}

// ---- OnTouchEnd_a24dc.cpp ----
int TouchSlider::OnTouchEnd(int param_1, int param_2)
{
    if (this->isDisabled != 0)
        return 0;

    int uVar1;
    if (this->isDragging == 0) {
        uVar1 = 0;
    } else {
        int iVar4 = this->numSteps;
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
    this->isDragging = 0;
    return uVar1;
}

// ---- draw_a2378.cpp ----
// PaintCanvas singleton holder (single pc-rel deref -> holder; object is *holder).
__attribute__((visibility("hidden"))) extern void **g_TouchSlider_canvas;

void TouchSlider::draw()
{
    void **holder = g_TouchSlider_canvas;
    int color = this->isDisabled != 0 ? 0xFFFFFF2F : -1;
    ((PaintCanvas*)*holder)->SetColor((unsigned int)color);
    ((PaintCanvas*)*holder)->DrawImage2D(this->trackImage, this->x, this->y);
    ((PaintCanvas*)*holder)->DrawImage2D(this->knobImage, this->knobX, this->knobY, (unsigned char)0x11, (unsigned char)0x44);
}

// ---- OnTouchMove_a2494.cpp ----
bool TouchSlider::OnTouchMove(int param_1, int param_2)
{
    if (this->isDisabled != 0)
        return false;
    if (this->isDragging != 0) {
        int half = this->knobWidth / 2;
        int lo = this->x + half;
        int hi = (this->x + this->trackWidth) - half;
        if (param_1 < hi)
            hi = param_1;
        if (hi <= lo)
            hi = lo;
        this->knobX = hi;
    }
    return this->isDragging != 0;
}

// ---- getValue_a23c0.cpp ----
float TouchSlider::getValue()
{
    float fVar1 = (float)(this->trackWidth - this->knobWidth);
    float fVar2 = (float)((this->knobX - this->knobWidth / 2) - this->x);
    return fVar2 / fVar1;
}

// ---- setHalfTransparent_a2426.cpp ----
void TouchSlider::setHalfTransparent(bool param_1)
{
    this->isDisabled = param_1;
}

// ---- touchedInside_a2448.cpp ----
int TouchSlider::touchedInside(int param_1, int param_2)
{
    int half = this->knobWidth >> 1;
    int cx = this->knobX;
    int pad = this->touchPadding;
    if (cx - half - pad > param_1)
        return 0;
    if (param_1 >= half + cx + pad)
        return 0;
    int half2 = this->knobHeight >> 1;
    int cy = this->knobY;
    if (cy - pad - half2 > param_2)
        return 0;
    return param_2 <= half2 + cy + pad;
}

// ---- setValue_a23ee.cpp ----
void TouchSlider::setValue(float param_1)
{
    float fVar1 = (float)(this->trackWidth - this->knobWidth);
    float fVar2 = (float)(this->x + this->knobWidth / 2);
    this->knobX = (int)(fVar2 + fVar1 * param_1);
}
