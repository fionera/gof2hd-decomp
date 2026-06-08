#include "class.h"

void MarqueeImage::draw(int x, int y)
{
    char *self = (char *)this;
    int32_t offset = *(int32_t *)(self + 0x20);
    *(int32_t *)(self + 0xc) = x;
    *(int32_t *)(self + 0x10) = y;

    register uint32_t visibleWidth;
    register int drawX = x;
    register int zero = 0;
    if (offset <= -1) {
        goto skippedFirst;
    }

    visibleWidth = (uint32_t)self;
    visibleWidth += 0x14;
    {
        int drawWidth = offset;
        if (drawWidth > *(int32_t *)visibleWidth) {
            drawWidth = *(int32_t *)visibleWidth;
        }

        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, *(uint32_t *)self, (int)*(float *)(self + 0x18),
                                 zero, drawWidth, *(int32_t *)(self + 8), 0LL, zero, drawX, y);
        offset = *(int32_t *)(self + 0x20);
    }
    goto afterFirst;

skippedFirst:
    visibleWidth = (uint32_t)(self + 0x14);

afterFirst:
    if (offset <= *(int32_t *)visibleWidth) {
        PaintCanvas_DrawRegion2D(*g_MarqueeImage_canvas, *(uint32_t *)self, 0, 0,
                                 *(int32_t *)visibleWidth - offset,
                                 *(int32_t *)(self + 8), 0LL, zero,
                                 offset + drawX, y);
    }
}
