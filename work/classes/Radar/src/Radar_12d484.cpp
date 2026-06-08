#include "class.h"

extern "C" void Radar_StringDefault(void *self);
extern "C" void Radar_MatrixDefault(void *self);
extern "C" void *Radar_operator_new(unsigned size);
extern "C" void Radar_ArrayStringCtor(void *self);
extern "C" void Radar_ArraySetLengthString(int length, void *self);
extern "C" void Radar_Image2DCreate(void *canvas, int id, void *out);
extern "C" int Radar_GetImage2DWidth(void *canvas, int image);
extern "C" int Radar_GetImage2DHeight(void *canvas, int image);

extern void *gRadarCanvasSlot;
extern void *gRadarLayoutSlot;

Radar::Radar(Level *level)
{
    char text6c[12];
    char local60[12];
    char text54[12];
    char local48[12];

    F<int>(this, 0x174) = 0;
    F<int>(this, 0x178) = 0;
    F<int>(this, 0x17c) = 0;
    F<int>(this, 0x180) = 0;
    F<int>(this, 0x164) = 0;
    F<int>(this, 0x168) = 0;
    F<int>(this, 0x16c) = 0;
    F<int>(this, 0x170) = 0;
    F<int>(this, 0x154) = 0;
    F<int>(this, 0x158) = 0;
    F<int>(this, 0x15c) = 0;
    F<int>(this, 0x160) = 0;

    Radar_StringDefault((char *)this + 0x18c);
    Radar_MatrixDefault((char *)this + 0x1d0);

    F<int>(this, 0x04) = 0;
    F<int>(this, 0x08) = 0;
    F<int>(this, 0x14) = 0;
    F<uint8_t>(this, 0x48) = 1;
    F<int>(this, 0x58) = 0;
    F<int>(this, 0x188) = 0;
    F<int>(this, 0x0c) = 0;
    F<int>(this, 0x10) = 0;
    F<int>(this, 0x18) = 0;
    F<int>(this, 0x38) = 0;
    F<int>(this, 0x3c) = 0;
    F<int>(this, 0x40) = 0;
    F<int>(this, 0x20c) = 0;
    F<int>(this, 0x1b4) = 0;
    F<int>(this, 0x1b8) = 0;
    F<int>(this, 0x1bc) = 0;
    F<uint8_t>(this, 0x54) = 0;
    F<uint8_t>(this, 0x130) = 0;
    F<uint8_t>(this, 0x120) = 0;
    F<int>(this, 0x11c) = 0;
    F<uint8_t>(this, 0x1a8) = 0;
    F<Level *>(this, 0x00) = level;

    void *layout = *(void **)gRadarLayoutSlot;
    if (layout != 0) {
        int width = F<int>(layout, 0xac);
        int height = F<int>(layout, 0xa8);
        F<int>(this, 0x21c) = width;
        F<int>(this, 0x220) = width >> 1;
        F<int>(this, 0x224) = height;
        F<int>(this, 0x228) = height >> 1;
        F<int>(this, 0x22c) = F<int>(layout, 0xa0);
        F<int>(this, 0x230) = F<int>(layout, 0xa4);
    }

    void *canvas = *(void **)gRadarCanvasSlot;
    Radar_Image2DCreate(canvas, 0x4c7, (char *)this + 0x1c4);
    int image = F<int>(this, 0x1c4);
    int imageWidth = Radar_GetImage2DWidth(canvas, image);
    int imageHeight = Radar_GetImage2DHeight(canvas, image);
    F<int>(this, 0x4c) = imageWidth;
    F<int>(this, 0x50) = imageHeight;
    F<int>(this, 0x114) = imageWidth * imageWidth;
    F<int>(this, 0x118) = imageHeight * imageHeight;
    F<float>(this, 0x10c) = 1.0f / (float)(imageWidth * imageWidth);
    F<float>(this, 0x110) = 1.0f / (float)(imageHeight * imageHeight);

    void *strings = Radar_operator_new(12);
    Radar_ArrayStringCtor(strings);
    F<void *>(this, 0x188) = strings;
    Radar_ArraySetLengthString(4, strings);

    (void)text6c;
    (void)local60;
    (void)text54;
    (void)local48;
}
