#include "class.h"

using AbyssEngine::AEMath::Vector;

struct Sparks {
    void translate(Vector const &v);
};

extern "C" void PaintCanvas_SpriteSystemAddPosition(void *canvas, uint32_t sprite, uint16_t index,
                                                     float x, float y, float z);

__attribute__((visibility("hidden"))) extern void **g_Sparks_canvas_translate;

void Sparks::translate(Vector const &v)
{
    void **canvas = g_Sparks_canvas_translate;
    for (uint32_t i = 0; i < F<uint32_t>(this, 0x18); i++)
        PaintCanvas_SpriteSystemAddPosition(*canvas, F<uint32_t>(this, 0x4), (uint16_t)i,
                                            v.x, v.y, v.z);
}
