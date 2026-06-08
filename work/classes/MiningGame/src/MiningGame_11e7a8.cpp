#include "class.h"

struct MiningGame {
    // @portable-fields
    unsigned char _pad_0[136];
    void* f_88; // 0x88
    void* f_8c; // 0x8c
    void* f_90; // 0x90
    void* f_94; // 0x94
    unsigned char _pad_98[56];
    void* f_d0; // 0xd0

    MiningGame(int layer, int station, Hud *hud);
};

__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenW;
__attribute__((visibility("hidden"))) extern int *g_MiningGame_screenH;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_layoutCtor;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_statusCtor;
__attribute__((visibility("hidden"))) extern void **g_MiningGame_canvasCtor;
__attribute__((visibility("hidden"))) extern void (*g_MiningGame_imageCreate)(void *canvas, int imageId, int *outId);
__attribute__((visibility("hidden"))) extern int (*g_MiningGame_imageWidth)(void *canvas, int imageId);
__attribute__((visibility("hidden"))) extern float *g_MiningGame_layerSpeed;

extern "C" void *MiningGame_Status_getShip(void *status);
extern "C" void *MiningGame_Ship_getFirstEquipmentOfSort(void *ship, int sort);
extern "C" int MiningGame_Item_getAttribute(void *item, int attribute);
extern "C" void MiningGame_PaintCanvas_Image2DCreate(void *canvas, int imageId, int *outId);
extern "C" int MiningGame_PaintCanvas_GetImage2DHeight(void *canvas, int imageId);
extern "C" int MiningGame_PaintCanvas_GetImage2DWidth(void *canvas, int imageId);
extern "C" void *MiningGame_operator_new(uint32_t size);
extern "C" void MiningGame_Sprite_ctor(void *sprite, int imageId, int width, int height);
extern "C" void MiningGame_Sprite_defineReferencePixel(void *sprite, int x, int y);
extern "C" void MiningGame_MarqueeImage_ctor(void *self, int imageId, int width, int x, int y, float value);
extern "C" void MiningGame_MarqueeImage_setSpeed(void *self, float speed);
extern "C" int MiningGame_Status_getCurrentCampaignMission(void *status);

MiningGame::MiningGame(int layer, int station, Hud *hud)
{
    int imageId[2];
    imageId[0] = -1;

    int *screenW = g_MiningGame_screenW;
    int *screenH = g_MiningGame_screenH;
    void **layoutHolder = g_MiningGame_layoutCtor;
    void **statusHolder = g_MiningGame_statusCtor;

    I(this, 0x18) = layer;
    I(this, 0x1c) = station;
    this->f_d0 = hud;

    void *layout = *layoutHolder;
    int centerX = *screenW >> 1;
    I(this, 0x58) = centerX;
    int centerY = I(layout, 0xd0) + (*screenH >> 1);
    I(this, 0x5c) = centerY;
    I(this, 0xc8) = I(layout, 0xd4);
    UC(this, 0x80) = layer == 7;
    I(this, 0x20) = 0;
    I(this, 0x24) = 0;
    I(this, 0x28) = 0;
    I(this, 0x6c) = 0;
    I(this, 0x70) = 0x9c4;
    I(this, 0x74) = 0;
    I(this, 0x78) = 0;
    I(this, 0x7c) = layer;
    *(uint16_t *)B(this, 0x81) = 0;
    UC(this, 0x83) = 0;
    F(this, 0x10) = (float)centerX;
    F(this, 0x00) = 0.0f;
    F(this, 0x04) = 0.0f;
    F(this, 0x08) = 0.0f;
    F(this, 0x0c) = 0.0f;
    F(this, 0x14) = (float)centerY;

    void *ship = MiningGame_Status_getShip(*statusHolder);
    void *equipment = MiningGame_Ship_getFirstEquipmentOfSort(ship, 0x13);
    if (equipment != 0) {
        int value = MiningGame_Item_getAttribute(equipment, 0x20);
        F(this, 0x2c) = 0.3f + ((float)value / 100.0f) * 1.5f;
        value = MiningGame_Item_getAttribute(equipment, 0x21);
        F(this, 0x28) = (float)value / 100.0f;
    }

    void **canvasHolder = g_MiningGame_canvasCtor;
    MiningGame_PaintCanvas_Image2DCreate(*canvasHolder, 0x4e6, imageId);
    int imageHeight = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, imageId[0]);
    void *sprite = MiningGame_operator_new(0x40);
    MiningGame_Sprite_ctor(sprite, imageId[0], imageHeight, imageHeight);
    this->f_94 = sprite;
    MiningGame_Sprite_defineReferencePixel(sprite, imageHeight / 2, imageHeight / 2);

    I(this, 0x68) = 0;
    void (*imageCreate)(void *, int, int *) = g_MiningGame_imageCreate;
    imageCreate(*canvasHolder, 0x4e2, (int *)B(this, 0xac));
    imageCreate(*canvasHolder, 0x4dd, (int *)B(this, 0xb0));
    imageCreate(*canvasHolder, 0x4de, (int *)B(this, 0xb4));
    imageCreate(*canvasHolder, 0x4e1, (int *)B(this, 0xb8));
    imageCreate(*canvasHolder, 0x4df, (int *)B(this, 0xbc));
    imageCreate(*canvasHolder, 0x4e0, (int *)B(this, 0xc0));
    imageCreate(*canvasHolder, 0x4e5, (int *)B(this, 0x9c));
    imageCreate(*canvasHolder, 0x4e4, (int *)B(this, 0xa0));
    imageCreate(*canvasHolder, 0x4e7, (int *)B(this, 0x98));
    imageCreate(*canvasHolder, 0x4e3, (int *)B(this, 0xa4));
    imageCreate(*canvasHolder, 0x4e8, (int *)B(this, 0xa8));
    imageCreate(*canvasHolder, 0x4ed, (int *)B(this, 0xc4));

    if (UC(this, 0x80) != 0) {
        int coreImage = 0x523;
        if (station == 0xa4) {
            coreImage = 0x522;
        }
        MiningGame_PaintCanvas_Image2DCreate(*canvasHolder, coreImage, (int *)B(this, 0x60));
    }

    I(this, 0x48) = MiningGame_PaintCanvas_GetImage2DWidth(*canvasHolder, I(this, 0xa8));
    I(this, 0x4c) = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0xa8));
    int x = *screenW / 2 - I(this, 0x48) / 2;
    I(this, 0x50) = x;
    int y = I(layout, 0xd8);
    I(this, 0x54) = y;

    void *leftMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(leftMarquee, 0x4eb, I(this, 0xc8), x, I(this, 0x4c) + y + 5, 20.0f);
    this->f_8c = leftMarquee;

    void *rightMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(rightMarquee, 0x4ec, I(this, 0xc8),
                                 (I(this, 0x50) - I(this, 0xc8)) + I(this, 0x48),
                                 I(this, 0x4c) + I(this, 0x54) + 5, 32.0f);
    this->f_90 = rightMarquee;

    int (*imageWidth)(void *, int) = g_MiningGame_imageWidth;
    I(this, 0x40) = imageWidth(*canvasHolder, I(this, 0x98)) / 2 + 5;
    I(this, 0x44) = imageWidth(*canvasHolder, I(this, 0x9c)) / 2;
    I(this, 0x34) = MiningGame_PaintCanvas_GetImage2DHeight(*canvasHolder, I(this, 0xa0));

    void *oreMarquee = MiningGame_operator_new(0x24);
    MiningGame_MarqueeImage_ctor(oreMarquee, 0x4e4, imageWidth(*canvasHolder, I(this, 0x9c)) - 8, 0, 0,
                                 F(layout, 0xdc));
    this->f_88 = oreMarquee;
    MiningGame_MarqueeImage_setSpeed(oreMarquee, F(layout, 0xe0) * g_MiningGame_layerSpeed[I(this, 0x78)]);

    I(this, 0xcc) = 0;
    F(this, 0x64) = 1.0f;
    UC(this, 0x84) = MiningGame_Status_getCurrentCampaignMission(*statusHolder) > 4;
}
