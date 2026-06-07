#include "class.h"

struct Sparks {
    void update(int step);
};

extern "C" void PaintCanvas_SpriteSystemSetSize(void *canvas, uint32_t sprite, uint16_t index,
                                                 int16_t size);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_update;

void Sparks::update(int step)
{
    if (F<uint8_t>(this, 0x10) == 0)
        return;

    int elapsed0 = F<int>(this, 0x0c);
    uint32_t i = 0;
    elapsed0 += step;
    F<int>(this, 0x0c) = elapsed0;
    for (; i < F<uint32_t>(this, 0x18); i++) {
        int *thresholds = F<int *>(this, 0x0);
        int elapsed = F<int>(this, 0x0c);
        if (elapsed <= thresholds[i])
            continue;
        int size = F<int>(this, 0x1c) - (elapsed << 1);
        PaintCanvas_SpriteSystemSetSize(*g_Sparks_canvas_update, F<uint32_t>(this, 0x4), (uint16_t)i,
                                        (int16_t)size);
    }

    int elapsed = F<int>(this, 0x0c);
    int kind = F<int>(this, 0x14);
    if (kind == 1) {
        if (elapsed <= 0x1f4) {
            int limit = F<int>(this, 0x1c);
            elapsed += elapsed;
            if (elapsed <= limit)
                return;
        }
    } else {
        if (elapsed <= 0x1f4)
            return;
    }

    F<volatile uint8_t>(this, 0x10) = 0;
    F<volatile int>(this, 0x0c) = 0;
}
