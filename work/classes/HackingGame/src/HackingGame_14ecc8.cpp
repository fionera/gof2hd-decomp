#include "class.h"

typedef void (*ImageCreateFn)(void *canvas, uint16_t image, uint32_t *out);

extern "C" void PaintCanvas_Image2DCreate(void *canvas, uint16_t image, uint32_t *out);
__attribute__((visibility("hidden"))) extern void **g_HackingGame_ctor_canvas;
__attribute__((visibility("hidden"))) extern ImageCreateFn g_HackingGame_ctor_imageCreate;

HackingGame::HackingGame(int type, int canvas, int rewardItem, int rewardAmount, int dockingIndex)
{
    I(this, 0x124) = type;
    int scaledType = type + type * 2;
    int savedRewardItem = rewardItem;
    int savedCanvas = canvas;
    int i = 0;
    uint32_t *slot = (uint32_t *)((char *)this + scaledType * 16 + 0x64);
    void **canvasHolder = g_HackingGame_ctor_canvas;
    while (true) {
        void *canvasObject = *canvasHolder;
        if (i == 6)
            break;
        PaintCanvas_Image2DCreate(canvasObject, (uint16_t)(i + 0x1f4a), slot - 6);
        PaintCanvas_Image2DCreate(*canvasHolder, (uint16_t)(i + 0x1f50), slot);
        ++slot;
        ++i;
    }

    ImageCreateFn create = g_HackingGame_ctor_imageCreate;
    create(*canvasHolder, 0x1f48, (uint32_t *)((char *)this + 0x114));
    create(*canvasHolder, 0x1f49, (uint32_t *)((char *)this + 0x10c));
    create(*canvasHolder, 0x1f47, (uint32_t *)((char *)this + 0x110));
    create(*canvasHolder, 0x1f46, (uint32_t *)((char *)this + 0x118));
    create(*canvasHolder, 0x1f44, (uint32_t *)((char *)this + 0x11c));
    create(*canvasHolder, 0x1f45, (uint32_t *)((char *)this + 0x120));

    I(this, 0) = savedCanvas;
    I(this, 0x134) = savedRewardItem;
    I(this, 0x138) = rewardAmount;
    I(this, 0x13c) = dockingIndex;
    reInit();
}
