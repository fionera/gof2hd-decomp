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

    this->f_174 = 0;
    this->f_178 = 0;
    this->f_17c = 0;
    this->f_180 = 0;
    this->f_164 = 0;
    this->f_168 = 0;
    this->f_16c = 0;
    this->f_170 = 0;
    this->f_154 = 0;
    this->f_158 = 0;
    this->f_15c = 0;
    this->f_160 = 0;

    Radar_StringDefault((char *)this + 0x18c);
    Radar_MatrixDefault((char *)this + 0x1d0);

    this->f_4 = 0;
    this->f_8 = 0;
    this->f_14 = 0;
    this->f_48 = 1;
    this->f_58 = 0;
    this->f_188 = 0;
    this->f_c = 0;
    this->f_10 = 0;
    this->f_18 = 0;
    this->f_38 = 0;
    this->f_3c = 0;
    this->f_40 = 0;
    this->f_20c = 0;
    this->f_1b4 = 0;
    this->f_1b8 = 0;
    this->f_1bc = 0;
    this->f_54 = 0;
    this->f_130 = 0;
    this->f_120 = 0;
    this->f_11c = 0;
    this->f_1a8 = 0;
    this->f_0 = level;

    void *layout = *(void **)gRadarLayoutSlot;
    if (layout != 0) {
        int width = layout->f_ac;
        int height = layout->f_a8;
        this->f_21c = width;
        this->f_220 = width >> 1;
        this->f_224 = height;
        this->f_228 = height >> 1;
        this->f_22c = layout->f_a0;
        this->f_230 = layout->f_a4;
    }

    void *canvas = *(void **)gRadarCanvasSlot;
    Radar_Image2DCreate(canvas, 0x4c7, (char *)this + 0x1c4);
    int image = this->f_1c4;
    int imageWidth = Radar_GetImage2DWidth(canvas, image);
    int imageHeight = Radar_GetImage2DHeight(canvas, image);
    this->f_4c = imageWidth;
    this->f_50 = imageHeight;
    this->f_114 = imageWidth * imageWidth;
    this->f_118 = imageHeight * imageHeight;
    this->f_10c = 1.0f / (float)(imageWidth * imageWidth);
    this->f_110 = 1.0f / (float)(imageHeight * imageHeight);

    void *strings = Radar_operator_new(12);
    Radar_ArrayStringCtor(strings);
    this->f_188 = strings;
    Radar_ArraySetLengthString(4, strings);

    (void)text6c;
    (void)local60;
    (void)text54;
    (void)local48;
}
